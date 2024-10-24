#include <iostream>
#include <cmath>
#include <fstream>
// #include <ios>

const int SampleRate = 44100;
const int bitDepth = 16;


class OscGenerator {
    float frequency, amplitude, angle = 0.0f, offset = 0.0;
    public :
        OscGenerator (float freq, float amp) : frequency(freq), amplitude(amp) {
            offset =  2 * M_PI * frequency / SampleRate; 
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


int main () {
    float duration = 0.5; //in seconds
    float freq = 440;
    std::ofstream audioFile;
    audioFile.open("waveform.wav", std::ios::binary);
    OscGenerator sinOscillator1 = OscGenerator(freq, 1);
    OscGenerator sinOscillator2 = OscGenerator(freq*2, 1);
    OscGenerator delay = OscGenerator(freq*2, 0);

    //header
    audioFile << "RIFF";
    audioFile << "----";
    audioFile << "WAVE";

    //format
    audioFile << "fmt ";
    writeToFile(audioFile, 16, 4);
    writeToFile(audioFile, 1, 2);
    writeToFile(audioFile, 1, 2);
    writeToFile(audioFile, SampleRate, 4);
    writeToFile(audioFile, SampleRate*bitDepth/8, 4);
    writeToFile(audioFile, 2, 2);
    writeToFile(audioFile, bitDepth, 2);

    //data chunk

    audioFile << "data";
    audioFile << "----";

    int preAudioPosition = audioFile.tellp();

    auto maxAmplitude =  ( pow(2, bitDepth - 1) - 1);
    for (int i = 0; i < SampleRate*duration ; i++) {
        auto sample = sinOscillator1.process();
        int intSample = static_cast<int> (sample * maxAmplitude);
        writeToFile(audioFile, intSample, 2);
    }

    for (int i = 0; i < SampleRate*duration ; i++) {
        auto sample = delay.process();
        int intSample = static_cast<int> (sample * maxAmplitude);
        writeToFile(audioFile, intSample, 2);
    }

    for (int i = 0; i < SampleRate*duration ; i++) {
        auto sample = sinOscillator2.process();
        int intSample = static_cast<int> (sample * maxAmplitude);
        writeToFile(audioFile, intSample, 2);
    }

    int postAudioPosition = audioFile.tellp();

    audioFile.seekp(preAudioPosition - 4);
    writeToFile(audioFile ,postAudioPosition - preAudioPosition, 4);

    audioFile.seekp(4, std::ios::beg) ;
    writeToFile(audioFile, postAudioPosition - 8, 4);

    audioFile.close();
    return 0;
}


