//
//  microSDIFTests.m
//  Tests
//
//  Created by Alex on 28/01/2018.
//  Copyright © 2018 Alex Nadzharov. All rights reserved.
//

#import <XCTest/XCTest.h>

#include "mSDIF.hpp"
#include <string>

@interface miniSDIFTests : XCTestCase

@end

@implementation miniSDIFTests

- (void)testTypes
{
    MSDIFType* t = MSDIFType::fromSignature("1TRC");

    XCTAssert(t);

    NSLog(@"description: %s", t->description().c_str());

    XCTAssertEqual(t->columnNames().size(), 4);
    XCTAssertEqual(t->dataType(), mTFloat4);

    t = MSDIFType::fromSignature("err");

    XCTAssert(t == 0);

    // while there are incomplete type descriptions:
    t = MSDIFType::fromSignature("1MRK");

    XCTAssert(t == 0);
}

- (void)testMatrixHeader
{
    MSDIFMatrixHeader h1;

    h1.setSignature("1TRC");
    h1.columns = 3;
    h1.rows = 7;
    h1.dataType = mTFloat4;

    XCTAssertEqual(h1.byteSize(), 4);

    MSDIFMatrixHeader h2 = h1;

    XCTAssertEqual(h2.rows, h1.rows);
    XCTAssertEqual(h2.columns, h1.columns);
    XCTAssertEqual(h2.dataType, h1.dataType);
    XCTAssert(strncmp(h2.signature, "1TRC", 4) == 0);

    XCTAssertEqual(h2.byteSize(), 4);
    // todo file
}

- (void)testMatrix
{
    MSDIFMatrix m1("1TRC", 7, 4, mTFloat4);

    XCTAssertEqual(m1.matrixDataSize(), 4 * 4 * 7);
    XCTAssertEqual(m1.paddingSize(), 0);

    float t[4];
    for (int i = 0; i < 4; i++)
        t[i] = 100 * i;

    m1.setValues<float*>(t);

    float* vt = m1.values<float*>();

    XCTAssert(vt);
    for (int i = 0; i < 4; i++)
        XCTAssert(vt[i] == 100 * i);

    if (vt)
        for (int i = 0; i < 4; i++)
            XCTAssertEqual(t[i], vt[i]);

    MSDIFMatrix m2("1NVT", 7, 4, mTChar);
    m2.setValues<std::string>("test string");

    printf("%i", strncmp(m2.values<std::string>().c_str(), "test string", 11));
    XCTAssert(strncmp(m2.values<std::string>().c_str(), "test string", 11));

    // todo file
}

- (void)testFrameHeader
{
    MSDIFFrameHeader h1;

    h1.setSignature("1TRC");
    h1.streamID = 1;
    h1.time = 0.5;

    MSDIFFrameHeader h2 = h1;

    XCTAssert(!strncmp(h2.signature, "1TRC", 4));
    XCTAssert(h2.streamID == 1);
    XCTAssertEqualWithAccuracy(h2.time, 0.5, 0.00001);
}

- (void)testFrame
{
    MSDIFFrame f1("1TRC", 0);

    XCTAssert(f1.matrixCount() == 0);

    MSDIFMatrix* m1 = new MSDIFMatrix("1TRC", 1, 4, mTFloat4);
    MSDIFMatrix* m2 = new MSDIFMatrix("1NVT", 1, 4, mTChar);

    f1.addMatrix(m1);
    XCTAssert(f1.matrixCount() == 1);
    f1.addMatrix(m2);
    XCTAssert(f1.matrixCount() == 2);

    f1.insertMatrix(0, m1);
    XCTAssert(f1.matrices().at(0) == m1);
    XCTAssert(f1.matrices().at(1) == m1);
    XCTAssert(f1.matrices().at(2) == m2);

    XCTAssertEqual(f1.matrices().size(), 3);

    f1.removeMatrixAt(1);
    XCTAssertEqual(f1.matrixCount(), 2);
    XCTAssert(f1.matrices().at(0) == m1);
    XCTAssert(f1.matrices().at(1) == m2);

    f1.removeMatrixAt(0);
    XCTAssertEqual(f1.matrixCount(), 1);

    f1.removeAllMatrices();
    XCTAssertEqual(f1.matrixCount(), 0);

    f1.removeMatrixAt(0);
    f1.removeAllMatrices();
    
    //
    f1.addMatrix(m1);
    f1.addMatrix(m2);
    
    auto arr = f1.matricesWithSignature("1TRC");
    XCTAssertEqual(arr.size(), 1);
    XCTAssertEqual(arr[0], m1);
    
}

- (void)testFileHeader
{
    MSDIFFileHeader f1;

    f1.setSignature("SDIF");
    f1.headerFrameSize = 8;
    f1.specificationVersion = 3;
    f1.padding = 0;

    MSDIFFileHeader f2 = f1;

    XCTAssert(f2.headerFrameSize == 8);
    XCTAssert(f2.specificationVersion == 3);
    XCTAssert(f2.padding == 0);
    XCTAssert(!strncmp(f2.signature, "SDIF", 4));
}

- (void)testFile
{
    MSDIFFile ff;
    
    XCTAssert(ff.frameCount() == 0);
    
    MSDIFFrame* f1 = new MSDIFFrame("1TRC",0);
    MSDIFFrame* f2 = new MSDIFFrame("1NVT",-1);
    
    ff.addFrame(f1);
    XCTAssert(ff.frameCount() == 1);
    ff.addFrame(f2);
    XCTAssert(ff.frameCount() == 2);
    
    ff.insertFrame(0, f1);
    XCTAssert(ff.frames().at(0) == f1);
    XCTAssert(ff.frames().at(1) == f1);
    XCTAssert(ff.frames().at(2) == f2);
    
    XCTAssertEqual(ff.frames().size(), 3);
    
    ff.removeFrameAt(1);
    XCTAssertEqual(ff.frameCount(), 2);
    XCTAssert(ff.frames().at(0) == f1);
    XCTAssert(ff.frames().at(1) == f2);
    
    ff.removeFrameAt(0);
    XCTAssertEqual(ff.frameCount(), 1);
    
    ff.removeAllFrames();
    XCTAssertEqual(ff.frameCount(), 0);
    
    ff.removeFrameAt(0);
    ff.removeAllFrames();
    
    //
    ff.addFrame(f1);
    ff.addFrame(f2);
    
    auto arr = ff.framesWithSignature("1TRC");
    XCTAssertEqual(arr.size(), 1);
    XCTAssertEqual(arr[0], f1);
    
    arr = ff.framesWithStreamID(-1);
    XCTAssertEqual(arr.size(), 1);
    XCTAssertEqual(arr[0], f2);
}

- (void)testReadSDIFFile
{
    std::string fName = "/Users/njazz/Documents/____ios/2017-07/miniSDIF/miniSDIFTests/test.sdif";

    MSDIFFile* newFile = new MSDIFFile;

    int i = (int)newFile->readFile(fName);

    NSLog(@"result %i", i);
    XCTAssert(i == meOK);

    //    char s[5];
    //    for (int i = 0; i < 4; i++)
    //        s[i] = newFile->header.signature[i];
    //    s[4] = '\0';
    //
    //    NSLog(@"header: %s size: %i version: %i", s, newFile->header.headerFrameSize, newFile->header.specificationVersion);
    //    NSLog(@"frame count: %lu", newFile->frames.size());
    //
    //    for (int j = 0; j < newFile->frames.size(); j++) {
    //
    //        char s[5];
    //        for (int i = 0; i < 4; i++)
    //            s[i] = newFile->frames.at(i).header.signature[i];
    //        s[4] = '\0';
    //
    //        NSLog(@"frame: %i type: %s matrix count: %i", j, s, newFile->frames.at(j).header.matrixCount);
    //    }

    NSLog(@"%s", newFile->info().c_str());
}

- (void)testWriteSDIFFile
{
    std::string fName = "/Users/njazz/Documents/____ios/2017-07/miniSDIF/miniSDIFTests/test.sdif";

    MSDIFFile* newFile = new MSDIFFile;

    newFile->readFile(fName);

    std::string wName = "/Users/njazz/Documents/____ios/2017-07/miniSDIF/miniSDIFTests/testWrite.sdif";
    newFile->writeFile(wName);
}

@end
