#!/usr/bin/env python3

import random
import thinkdsp
from pydub import AudioSegment
from pydub.playback import play

# Standard note name and their frequencies (Hz)
noteFreqs = {
    "C0" : 16.35, "Db0" : 17.32, "D0" : 18.35, "Eb0" : 19.45,
    "E0" : 20.60, "F0" : 21.83, "Gb0" : 23.12, "G0" : 24.50,
    "Ab0" : 25.96, "A0" : 27.50, "Bb0" : 29.14, "B0" : 30.87,

    "C1" : 32.70, "Db1" : 34.65, "D1" : 36.71, "Eb1" : 38.89,
    "E1" : 41.20, "F1" : 43.65, "Gb1" : 46.25, "G1" : 49.00,
    "Ab1" : 51.91, "A1" : 55.00, "Bb1" : 58.27, "B1" : 61.74,

    "C2" : 65.41, "Db2" : 69.30, "D2" : 73.42, "Eb2" : 77.78,
    "E2" : 82.41, "F2" : 87.31, "Gb2" : 92.50, "G2" : 98.00,
    "Ab2" : 103.83, "A2" : 110.00, "Bb2" : 116.54, "B2" : 123.47,

    "C3" : 130.81, "Db3" : 138.59, "D3" : 146.83, "Eb3" : 155.56,
    "E3" : 164.81, "F3" : 174.61, "Gb3" : 185.00, "G3" : 196.00,
    "Ab3" : 207.65, "A3" : 220.00, "Bb3" : 233.08, "B3" : 246.94,

    "C4" : 261.63, "Db4" : 277.18, "D4" : 293.66, "Eb4" : 311.13,
    "E4" : 329.63, "F4" : 349.23, "Gb4" : 369.99, "G4" : 392.00,
    "Ab4" : 415.30, "A4" : 440.00, "Bb4" : 446.16, "B4" : 493.88,

    "C5" : 523.25, "Db5" : 554.37, "D5" : 587.33, "Eb5" : 622.25,
    "E5" : 659.25, "F5" : 698.46, "Gb5" : 739.99, "G5" : 783.99,
    "Ab5" : 830.61, "A5" : 880.00, "Bb5" : 932.33, "B5" : 987.77,
}

randomCoeffs = []
for i in range( 0, 8 ):
    randomCoeffs.append( random.uniform( -1, 1 ) )

fourierCoeffs = {
    "sine" : [ 0, 1, 0, 0, 0, 0, 0, 0 ],
    "sawtooth" : [ 0, 0.6366, 0, -0.2122, 0, 0.1273, 0, -0.0909 ],
    "trumpet" : [ 0.1155, 0.3417, 0.1789, 0.1232, 0.0678,
                  0.0473, 0.0260, 0.0045, 0.0020 ],
    "random" : randomCoeffs
}

def createNote( noteName="A4", type="sine", amp=0.5, beats=1.0, filter=None,
                cutoff=None, filename="defaultFilename"):
    frequency = noteFreqs[ noteName ]
    duration = beats / 2
    signal = thinkdsp.SinSignal( freq=0 )

    for i in range( 0, 8 ):
        signal += thinkdsp.SinSignal( freq=( frequency * i ),
                                      amp=amp*fourierCoeffs[ type ][ i ],
                                      offset=0 )

    wave = signal.make_wave( duration=duration, start=0, framerate=44100 )
    wave.write( filename=filename )
    audio = AudioSegment.from_wav( filename )
    print( "Creating note " + noteName + " at " + str( frequency ) +
           " for " + str( beats ) + " beats with the random synthesizer!" )

    if filter == "lowPass":
        audio = audio.low_pass_filter( cutoff )
        print( "Applying Low-Pass filter" )
    if filter == "highPass":
        audio = audio.high_pass_filter( cutoff )
        print( "Applying High-Pass filter" )

    return audio

A4_trumpet = createNote( noteName="A4", type="trumpet", amp=1.0, beats=4.0,
                         filter=None, cutoff=None )
A4_distorted = A4_trumpet[ :len( A4_trumpet ) ].overlay(
        A4_trumpet[ :len( A4_trumpet ) ] )
A4_highPass = createNote( noteName="A4", type="trumpet", amp=1.0, beats=4.0,
                          filter="highPass", cutoff=5000 )
#play( A4_trumpet )
#play( A4_distorted )
play( A4_highPass )

