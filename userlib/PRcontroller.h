#ifndef __PR_CONTROLLER_H__
#define __PR_CONTROLLER_H__

typedef struct{
    float x[4];
    float y[4];
    float cxx1;
    float cy1;
    float cy2;
    float cx0;
    float cx1;
    float cx2;
}PR_t;

#define pi (float)3.14159265f
#define kr (float) 50.0f    //系数Kr
#define kp (float) 0.0f   //系数Kp
#define wc (float)(2*2*pi)    // QPR控制器截止频率
#define w0 (float)(2*50*pi)   //QP控制器角频率 此处为50Hz
#define Ts (float)(1.0f/25000.0f)   //数字控制器采样周期 此处采样频率为25kHz
//tustin continuous 2 discrete coefficient  // QPR控制器的离散化系数

inline void PR_Init(PR_t* PR)
{
    PR->cx0 = 2*(Ts);
    PR->cy1 = (2.0*Ts*Ts*w0*w0 - 8.0)/(Ts*Ts*w0*w0 + 4.0*wc*Ts + 4.0);
    PR->cy2 = (Ts*Ts*w0*w0 + 4.0- 4.0*Ts*wc )/(Ts*Ts*w0*w0 + 4.0*wc*Ts + 4.0);
    PR->cx0 = (4.0*kp + Ts*Ts*kp*w0*w0 + 4.0*Ts*kp*wc + 4.0*Ts*kr*wc)/(Ts*Ts*w0*w0 + 4.0*wc*Ts + 4.0);
    PR->cx1 = (2.0*kp*Ts*Ts*w0*w0 - 8.0*kp)/(Ts*Ts*w0*w0 + 4.0*wc*Ts + 4.0);
    PR->cx2 = (4.0*kp+Ts*Ts*kp*w0*w0 - 4.0*Ts*kp*wc - 4.0*Ts*kr*wc)/(Ts*Ts*w0*w0 + 4.0*wc*Ts + 4.0);
}

//需要每次计算迭代的差分方程
//PR控制器输入为x
//PR控制器输出为p->y[0]
//控制器其他状态参数在结构体p中
inline void AAL_Control_Controller(PR_t *p,float x) // QPR电流控制器的离散化
{
    p->x[0] = x;

    //tustin 离散化PR控制
    p->y[0] =  -(p->cy1)*(p->y[1]) - p->cy2*(p->y[2]) + p->cx0*(p->x[0]) + p->cx1*(p->x[1]) + p->cx2*(p->x[2]);
    //限幅代码
    if(p->y[0]>0.475f)p->y[0]=0.475f;
    if(p->y[0]<-0.475f)p->y[0]=-0.475f;
        //更新数据
    p->x[3] = p->x[2];
    p->x[2] = p->x[1];
    p->x[1] = p->x[0];

    p->y[3] = p->y[2];
    p->y[2] = p->y[1];
    p->y[1] = p->y[0];
}
//用法示例
//定义控制器实例
//Controller_Structure PR_Controller;
//迭代循环
// {
// AAL_Control_Controller(&PR_Controller,给定值-测量值);
// //PR控制器输出
// PR_Controller.y[0];
// }

#endif