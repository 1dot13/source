#include "BufferMan.h"

// Bounds-checked data loading from a buffer for save/load operations.
//
// Usage:
//   LoadDataFromBuffer(dst, src, size, bufStart, bufSize, "label");
//     - dst: destination pointer
//     - src: current read position
//     - size: number of bytes to load
//     - bufStart: start of the buffer
//     - bufSize: total size of the buffer
//     - "label": name of the data being loaded (for error messages)
//   Returns TRUE on success. On failure, logs a DebugMsg and returns FALSE.
BOOLEAN LoadDataFromBuffer(PTR dst, CHAR8*& src, INT32 size, CHAR8* bufStart, INT32 bufSize, const CHAR8* label)
{
    // src is a reference to the read cursor; use its value (not &src, which would be the
    // address of the pointer itself) to locate it inside the buffer.
    INT32 offset = (INT32)(src - bufStart);
    if ( offset < 0 || offset + size > bufSize ) {
        gfBufferLoadFailed = TRUE;
        DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("Save file corrupted: not enough data to load %s at offset %d", label, offset));
        return FALSE;
    }
    memcpy(dst, src, size);
    src = src + size;
    return TRUE;
}

// 4-arg convenience form that uses the global LoadBuffer bounds. Used in LoadSavedGame() and its subfunctions.
BOOLEAN LoadDataFromBuffer(PTR dst, CHAR8*& src, INT32 size, const CHAR8* label)
{
    return LoadDataFromBuffer(dst, src, size, gpLoadBufferStart, giLoadBufferSize, label);
}
