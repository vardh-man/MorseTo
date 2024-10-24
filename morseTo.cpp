#include <iostream>
#include <string>
#include <fstream>

void toAudio(std::string alphabet, float duration, std::string filename);

int main (int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "MorseTo [options] filename";
    } else {
        std::string option1 = argv[1];
        if (option1 == "--audio" || option1 == "-a") {
            std::string filename;
            float duration;
            if (argv[2]){
                filename = argv[2];
                if (argv[3] == "auto") {
                    duration = 0.5;
                } else if(argv[3]){
                    duration = std::atof(argv[3]);
                } else {
                    std::string durStr;
                    std::cout << "enter the duration of each sound: " ;
                    std::cin >> durStr;
                    duration = std::atof(durStr.c_str());
                }
            } else {
                system("vi ./defaultInp.txt");
                filename = "defaultInp.txt";
                duration = 0.5;
            }
            std::ifstream file;
            std::string alphabet;
            std::string line;
            file.open(filename);
            if(file.is_open()) {
                while(std::getline(file, line)){
                    alphabet += line + "\n";
                }
            }
            toAudio(alphabet, duration, filename);
            file.close();
        }
     }
     return 0;
}