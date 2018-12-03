#ifndef __SOI3_H_
#define __SOI3_H_

#define indexZ buf[MAX]
#define indexO buf[MAX+1]
#define indexZpri buf_pri[MAX]
#define indexOpri buf_pri[MAX+1]

#define MAX 400
#define KEY 45356
#define SMH_START 0x7f80dc9b0000

void decrease(int semid, int semnum);
void increase(int semid, int semnum);

#endif //__SOI3_H_