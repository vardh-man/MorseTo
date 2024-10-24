#include <vector>
#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>

std::string toMorse (std::string alphabet) {
    transform(alphabet.begin(), alphabet.end(), alphabet.begin(), ::toupper);
    std::cout << alphabet << std::endl;
    std::vector<std::string> morseCode = {{".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....","..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.","--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-","-.--", "--.."}};
    std::string morse = "";
    for (int i = 0; i < alphabet.length(); i++) {
        int c = alphabet[i];
        if (c >= 65 && c <= 90){
            int ind = alphabet[i] - 65;
            morse += morseCode[ind];
        } else {
            morse += alphabet[i];
        }
    }
    std::cout << morse;
    return  morse;
}

