#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <cctype>
#include <algorithm>

int sampleRate = 44100;
const int bitDepth = 16;
auto maxAmplitude =  ( pow(2, bitDepth - 1) - 1);
float freq = 440;


class OscGenerator {
    float frequency, amplitude, angle = 0.0f, offset = 0.0;
    public :
        OscGenerator (float freq, float amp) : frequency(freq), amplitude(amp) {
            offset =  2 * M_PI * frequency / sampleRate; 
        }

        float process () {
            auto sample = amplitude * sin (angle);
            angle += offset;
            return sample;

    }
};

void writeToFile (std::ofstream &file, int value, int size) {
    file.write(reinterpret_cast<char*> (&value), size);
}

void delay (OscGenerator delay, float duration, std::ofstream &audioFile) {
    for (int i = 0; i < sampleRate*duration ; i++) {
        auto sample = delay.process();
        int intSample = static_cast<int> (sample * maxAmplitude);
        writeToFile(audioFile, intSample, 2);
    }
}
void dits (OscGenerator dits, float duration, std::ofstream &audioFile) {
    for (int i = 0; i < sampleRate*duration ; i++) {
        auto sample = dits.process();
        int intSample = static_cast<int> (sample * maxAmplitude);
        writeToFile(audioFile, intSample, 2);
    }
}
void dahs (OscGenerator dahs, float duration, std::ofstream &audioFile) {
    for (int i = 0; i < sampleRate*duration ; i++) {
        auto sample = dahs.process();
        int intSample = static_cast<int> (sample * maxAmplitude);
        writeToFile(audioFile, intSample, 2);
    }
}

std::string parseFilename(std::string filename) {
    std::string newFilename;
    for (int i = 0; i < filename.length(); i++) {
        if (filename[i] == '.') {
            return newFilename;
        } else {
            newFilename += filename[i];
        }
    }
    return newFilename ;
}


std::string toMorse(std::string alphabet);

//main to audio function

void toAudio (std::string alphabet, float duration, std::string filename = "waveform") {
    std::string morse = toMorse(alphabet);
    std::ofstream audioFile;
    filename = parseFilename(filename) + ".wav";
    std::string filepath = "./waveforms/" + filename;
    audioFile.open(filepath.c_str(), std::ios::binary);

    //header
    audioFile << "RIFF";
    audioFile << "----";
    audioFile << "WAVE";

    //format
    audioFile << "fmt ";
    writeToFile(audioFile, 16, 4);
    writeToFile(audioFile, 1, 2);
    writeToFile(audioFile, 1, 2);
    writeToFile(audioFile, sampleRate, 4);
    writeToFile(audioFile, sampleRate*bitDepth/8, 4);
    writeToFile(audioFile, 2, 2);
    writeToFile(audioFile, bitDepth, 2);

    //data chunk

    audioFile << "data";
    audioFile << "----";

    int preAudioPosition = audioFile.tellp();

    OscGenerator dly = OscGenerator(freq*2, 0);
    OscGenerator dot = OscGenerator(freq*2, 1);
    OscGenerator dash = OscGenerator(freq, 1);

    for (int i = 0; i < morse.length(); i++) {
        if (morse[i] == '.') {
            dits(dot, duration, audioFile);
            delay(dly, duration, audioFile);
        } else if (morse[i] == '-') {
            dahs(dash, duration, audioFile);
            delay(dly, duration, audioFile);
        } else if (morse[i] == ' ' || morse[i] == '\n') {
            delay(dly, duration*2, audioFile);
        }
    }
    int postAudioPosition = audioFile.tellp();

    audioFile.seekp(preAudioPosition - 4);
    writeToFile(audioFile ,postAudioPosition - preAudioPosition, 4);

    audioFile.seekp(4, std::ios::beg) ;
    writeToFile(audioFile, postAudioPosition - 8, 4);

    audioFile.close();

}

