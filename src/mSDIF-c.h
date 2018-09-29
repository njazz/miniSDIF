#ifndef mSDIFFile_h
#define mSDIFFile_h

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

typedef void* m_sdif_frame;
typedef void* m_sdif_matrix;
typedef void* m_sdif_file;

typedef int m_file_error;

// Matrix

// Frame

// File

m_sdif_file m_sdif_file_new();

m_file_error m_sdif_file_read_file(char* filename);
m_file_error m_sdif_file_write_file(char* filename);

m_sdif_frame* m_sdif_file_frames();

m_sdif_frame* m_sdif_file_framesframesWithSignature(char* signature);
m_sdif_frame* m_sdif_file_framesframesWithNotSignature(char* signature);
m_sdif_frame* m_sdif_file_framesframesWithTimeRange(double start, double end);
m_sdif_frame* m_sdif_file_framesframesWithStreamID(uint32_t streamID);

uint32_t m_sdif_file_frame_count();

void m_sdif_file_addFrame(m_sdif_frame fr);
void m_sdif_file_removeFrame(m_sdif_frame fr);
void m_sdif_file_removeFrameAt(size_t idx);
void m_sdif_file_insertFrame(size_t idx, m_sdif_frame fr);

void m_sdif_file_remove_all_frames();
void m_sdif_file_remove_frames_with_signature(char* signature);

void m_sdif_file_replace_frames(MSDIFFrameVector fv) { _frames = fv; }

char* m_sdif_file_info();

// editing
void m_sdif_file_merge_frames_with_signature(char* signature, m_sdif_file file);
void m_sdif_file_sort_frames_by_time();

//    void reverse();
void m_sdif_file_apply_time();
//    void applyGain();

// non-destructive editing:
// time
void m_sdif_file_set_time_offset(float t_o);
void m_sdif_file_set_time_scale(float t_s);

float m_sdif_file_time_offset();
float m_sdif_file_time_scale();



// ==========
//template <typename T>
//void createFrameWithMatix(char * signature, int streamID, float time, T data, int rows)
//{
//    MSDIFMatrix* m = new MSDIFMatrix(signature, rows);
//    m->setValues<T>(data);
//    m_sdif_frame f = new MSDIFFrame(signature, streamID);
//    f->setTime(time);
//    f->addMatrix(m);
//    addFrame(f);
//}

//template <typename T>
//void insertFrameWithMatix(size_t idx, char * signature, int streamID, float time, T data, int rows)

#ifdef __cplusplus
}
#endif

#endif
