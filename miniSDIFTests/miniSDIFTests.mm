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
    MSDIFMatrix m1("1TRC",3,7,mTFloat4);

    XCTAssertEqual(m1.matrixDataSize(), 4 * 3 * 7);
    XCTAssertEqual(m1.paddingSize(), 4);

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
    XCTAssert(h2.streamID==1);
    XCTAssertEqualWithAccuracy(h2.time, 0.5, 0.00001);
}

- (void)testFrame
{
}

- (void)testFileHeader
{
    MSDIFFileHeader f1;
    
    f1.setSignature("SDIF");
    f1.headerFrameSize = 8;
    f1.specificationVersion =3;
    f1.padding = 0;
    
    MSDIFFileHeader f2 = f1;
    
    XCTAssert(f2.headerFrameSize == 8);
    XCTAssert(f2.specificationVersion == 3);
    XCTAssert(f2.padding == 0);
    XCTAssert(!strncmp(f2.signature,"SDIF",4));
}

- (void)testFile
{
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
