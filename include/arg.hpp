#ifndef ARG_H
#define ARG_H

class Arg {

  public:
    static int get_port_when_valid(int ac, char **av);
    static bool invalid_port(int port);

  private:
    static const int MIN_ARG_NUM;
    static const int MIN_PORT_NUM;
    static const int MAX_PORT_NUM;

};

#endif // !ARG_H
