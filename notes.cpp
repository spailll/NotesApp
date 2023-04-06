//Notes app     By: Ben Sailor
#include <iostream>                         //Included Libraries
#include <string>
#include <filesystem>
#include <fstream>
#include <cstdlib>

void printFile(std::string path, std::string fileName);
void listFiles(std::string path); 

int main(int argc, char** argv) {
  
  //handling args
  const std::string userName(std::getenv("USER"));
  const std::string absolute_path = "/home/" + userName + "/Documents/Notes";
  bool noArgs = true;                       //Boolean for detecting a no arguments call
  
  for (int i = 1; i < argc; ++i) {          //Loop to iterate through arguments
    std::string arg = argv[i];
    noArgs = false;

    if (arg == "-l" || arg == "--list") {   //List out the possible notes to view
      listFiles(absolute_path);

    }   else if (arg == "-f" || arg == "--file") {  //Notes file to be viewed
      i++;
      if (i >= argc) { //ERROR: No File Path Specified
        std::cerr << "Error: " << "No File Path Specified with -f flag" << std::endl;
        return 1;
      }
      printFile(absolute_path, argv[i]);
      return 1;
    }   else {
      std::cerr << "Error: " << "Unknown flag" << std::endl;
    }
  }

  if (noArgs) { //manual prompt
    listFiles(absolute_path);
    std::cout << "\nPlease enter the name of the notes document you would like to view.\n\t>> ";
    std::string fileName;
    std::cin >> fileName;
    printFile(absolute_path, fileName);
  }
  return 1;
}

void printFile(std::string path, std::string fileName) {
  std::ifstream inputFile(path + "/" + fileName);
  if (!(inputFile.is_open())) {
    std::cerr << "Error: " << "Unable to open notes at " << fileName << std::endl;
    return;
  }
  std::string line;
  while (std::getline(inputFile, line)) {
    std::cout << line << std::endl;
  }
  inputFile.close();
  return;
}


void listFiles(std::string absolute_path) {
  try {
    int pathLength = absolute_path.length();
    for (const auto& entry : std::filesystem::directory_iterator(absolute_path)) {
      std::string entryPath = entry.path();
      std::string member = entryPath.substr(pathLength + 1, entryPath.length() - pathLength - 1);
      if (member.at(0) != '.' && member != "README.md") {        //Check if the file is hidden with .
        std::cout << member << std::endl;
      }
    }
    return;
  } catch (const std::filesystem::filesystem_error& e) { //FIXME:
    std::cerr << "Error: " << e.what() << std::endl;
    return;
  }
}
