#ifndef DETAILS_H
#define DETAILS_H
#include <string>

class Details {
private:
  std::string site;
  std::string username;
  std::string password;

public:
  Details(const std::string &site, const std::string &username,
          const std::string &password)
      : site(site), username(username), password(password) {}

  std::string getsite() const { return site; }
  std::string getUsername() const { return username; }
  std::string getPassword() const { return password; }

  void setSite(const std::string &s) { site = s; }
  void setUsername(const std::string &u) { username = u; }
  void setPassword(const std::string &p) { password = p; }
};

#endif