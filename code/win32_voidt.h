/*******************************************************************
** Copyright (C) 2015-2016 {Joey de Vries} {joey.d.vries@gmail.com}
**
** This code is part of Voidt.
** https://github.com/JoeyDeVries/Voidt
**
** Voidt is free software: you can redistribute it and/or modify it
** under the terms of the CC BY-NC 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
*******************************************************************/
#ifndef WIN32_VOIDT_H
#define WIN32_VOIDT_H


// ----------------------------------------------------------------------------
//      STRUCTS
// ----------------------------------------------------------------------------
// screen buffer data
struct win32_offscreen_buffer
{
    BITMAPINFO  Info;
    void       *Memory;
    int32      Width;
    int32      Height;
    uint32      Pitch;
    uint8       BytesPerPixel;
    uint8       Padding[3]; // for 32 byte alignment
};

// represents the window dimension return value(s)
struct win32_window_dimension
{
    int32 Width;
    int32 Height;
};

// direct sound audio buffer data
struct win32_sound_output
{
    uint32 runningSampleIndex;
    int    samplesPerSecond;     
    int    bytesPerSample;    
    DWORD  secondaryBufferSize;    
    // int    latencySampleCount; // how many samples ahead of the play cursor we'd like to be
    DWORD  SafetyBytes;
};

struct win32_game_code
{
    HMODULE GameCodeDLL;
    FILETIME DLLLastWriteTime;
    game_update_and_render *UpdateAndRender;
    game_get_sound_samples *GetSoundSamples;
    
    bool32 IsValid;
};

struct win32_replay_buffer
{
    void *MemoryBlock;
};
struct win32_state 
{
    uint64 TotalSize;
    void* GameMemoryBlock;
    win32_replay_buffer ReplayBuffers[4];
    
    HANDLE RecordingHandle;
    int InputRecordingIndex;
    HANDLE PlaybackHandle;
    int InputPlayingIndex;
    
    char EXEFileName[MAX_PATH];
    char *OnePastLastEXEFileNameSlash;
    
};

// ----------------------------------------------------------------------------
//      GLOBALS
// ----------------------------------------------------------------------------
global_variable bool32 GlobalRunning;
global_variable win32_offscreen_buffer GlobalBackBuffer;
global_variable LPDIRECTSOUNDBUFFER GlobalSecondaryBuffer;
global_variable LARGE_INTEGER GlobalPerfCountFrequency;
global_variable bool32 DEBUGGlobalShowCursor;
global_variable WINDOWPLACEMENT GlobalWindowPosition = { sizeof(GlobalWindowPosition) };


// ----------------------------------------------------------------------------
//      FUNCTION POINTER DEFINITIONS (LIBRARY HOOKS)
// ----------------------------------------------------------------------------
// XInputGetState (dynamic DLL binding without linking)
// this ensures we can use the XInput functions without requiring the DLL during execution
// we declared a stub version of each function in case we can't find any functions
#define X_INPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE *pState)
typedef X_INPUT_GET_STATE(x_input_get_state);
X_INPUT_GET_STATE(XInputGetStateStub)
{
    return ERROR_DEVICE_NOT_CONNECTED;
}
global_variable x_input_get_state *XInputGetState_ = XInputGetStateStub;
#define XInputGetState XInputGetState_

// XInputSetState (dynamic DLL binding without linking)
#define X_INPUT_SET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration)
typedef X_INPUT_SET_STATE(x_input_set_state);
X_INPUT_SET_STATE(XInputSetStateStub)
{
    return ERROR_DEVICE_NOT_CONNECTED;
}
global_variable x_input_set_state *XInputSetState_ = XInputSetStateStub;
#define XInputSetState XInputSetState_

// DirectSoundCreate
#define DIRECT_SOUND_CREATE(name) HRESULT WINAPI name(LPCGUID pcGuidDevice, LPDIRECTSOUND *ppDS, LPUNKNOWN pUnkOuter)
typedef DIRECT_SOUND_CREATE(direct_sound_create);


#endif