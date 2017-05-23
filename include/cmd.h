#ifndef CMD_H
#define CMD_H
namespace Game {
struct Cmd {
  int id; //指令针对对象
  float x;
  float z;
  int type;     //指令类型

  Cmd(int t_, int i_, float x_, float z_) 
    : type(t_), id(i_), x(x_), z(z_)  {}
};
}
#endif