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
#ifndef SOUND_H
#define SOUND_H

struct Sound 
{
    uint32 ChannelCount;
    uint32 SampleCount;
    int16 *Samples[2];           
};

#include "wav.h"
#include "wav.cpp"

struct PlayingSound
{
    // TODO(Joey): replace with unique Asset ID scheme | GUID
    // I don't want to chase too much pointers.
    Sound *Source; 
    
    r32 SamplesPlayed;
    b32 Loop;
    r32 CurrentVolume[2];
    r32 TargetVolume[2];
    r32 dVolume[2];
    r32 Pitch;
        
    PlayingSound *Next;
};

struct SoundMixer 
{   
    memory_arena MixerArena;
    
    r32 MasterVolume;

    PlayingSound *FirstPlayingSound;
    PlayingSound *FirstFreePlayingSound; // NOTE(Joey): re-use previously de-queued playing sounds memory    
};


#endif