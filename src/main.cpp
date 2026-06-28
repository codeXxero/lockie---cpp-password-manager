#include "passfunctions.h"

#include <iostream>
#include <string>

using std::cout;
int main(int argc, char *argv[]) {

  password_functions passfunction;
  passfunction.load_details();

  int direction = verify_entry();

  if (argc == 1) {
    passfunction.choice_direction(direction,
                                  [&]() { passfunction.menu_driven(); });
  } else {

    std::string command = argv[1];
    if (argc == 5 && command == "add") {

      passfunction.choice_direction(direction, [&]() {
        passfunction.add_CLI(argv[2], argv[3], argv[4]);
      });
    }

    else if (argc == 3) {
      if (command == "search") {
        passfunction.choice_direction(
            direction, [&]() { passfunction.search_CLI(argv[2]); });
      } else if (command == "delete") {

        passfunction.choice_direction(
            direction, [&]() { passfunction.delete_CLI(argv[2]); });
      } else if (command == "edit") {
        passfunction.choice_direction(
            direction, [&]() { passfunction.edit_CLI(argv[2]); });
      } else {
        cout << "invalid command";
      }
    } else if (argc == 2) {

      if (command == "clearall") {

        passfunction.choice_direction(direction,
                                      [&]() { passfunction.delete_all(); });
      } else if (command == "listall") {
        passfunction.choice_direction(direction,
                                      [&]() { passfunction.show_all(); });
      } else if (command == "changemasterkey") {

        passfunction.choice_direction(direction, [&]() { change_masterkey(); });
      } else if (command == "add") {
        passfunction.choice_direction(direction,
                                      [&]() { passfunction.add_details(); });

      } else {
        cout << "invalid command";
      }

    } else {
      cout << "invalid command\n";
    }
  }
}