#include "passfunctions.h"
#include <iostream>
#include <string>

using std::cout;

int main(int argc, char *argv[]) {
  if (sodium_init() < 0) {
    cout << "Failed to initialize libsodium\n";
    return 1;
  }

  std::string userKey;
  int direction = verify_entry(userKey); // userKey is filled on success

  if (direction == 2)
    return 0; // first run, restart needed
  if (direction == 1) {
    cout << "Wrong master key. Access denied.\n";
    return 1;
  }

  // direction == 0: authenticated
  password_functions passfunction;
  passfunction.deriveKey(userKey); // derive 32-byte encryption key
  passfunction.load_details();     // decrypt and load vault

  if (argc == 1) {
    passfunction.menu_driven();
  } else {
    std::string command = argv[1];

    if (argc == 5 && command == "add") {
      passfunction.add_CLI(argv[2], argv[3], argv[4]);
    } else if (argc == 3) {
      if (command == "search")
        passfunction.search_CLI(argv[2]);
      else if (command == "delete")
        passfunction.delete_CLI(argv[2]);
      else if (command == "edit")
        passfunction.edit_CLI(argv[2]);
      else
        cout << "invalid command\n";
    } else if (argc == 2) {
      if (command == "clearall")
        passfunction.delete_all();
      else if (command == "listall")
        passfunction.show_all();
      else if (command == "changemasterkey")
        change_masterkey();
      else if (command == "add") {
        passfunction.add_details();
      } else
        cout << "invalid command\n";
    } else {
      cout << "invalid command\n";
    }
  }

  return 0;
}