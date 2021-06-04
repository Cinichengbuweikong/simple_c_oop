#include <stdio.h>
typedef struct shapeclass shape; //将class结构体定义为shape类
typedef struct rectclass rect; //将rectclass结构体定义为rect类
typedef struct polyclass poly; //将polyclass结构体定义为poly类
typedef struct squareclass square; //将squareclass结构体定义为square类


struct vtable //定义一个结构体 里面有函数 这个结构体可以通过指向子类来实现多态
{
    void (* dtfun)(shape);
};


//封装
//封装的核心就是用结构体把属性和方法(函数指针代替)放在一起
struct shapeclass
{
    //属性
    int width;
    int height;
    char* color;

    //方法
    void (*show)(shape);

    //用结构体指针实现的多态
    struct vtable* vpointer;
};

//实现show方法
void shape_show(shape self)
{
    printf("width: %d\n", self.width);
    printf("height: %d\n", self.height);
    printf("color: %s\n", self.color);
}

//一个空函数 在shape类的初始化里给vpointer占位
void shape_emptyfunc(shape self)
{
    ;
}

struct vtable vt = {shape_emptyfunc}; //将空函数装入结构体以便调用

//shape类的初始化方法
shape shape_init(shape self, int width, int height, char *color, void (* show)(shape))
{
    self.width = width;
    self.height = height;
    self.color = color;
    self.show = show;
    self.vpointer = &vt;

    return self;
}




//继承
//继承就是在自己的结构体里放一个父类的结构体对象
struct rectclass
{
    //继承
    shape super_shape; //shape父类

    //rect类的属性
    int position_x;
    int position_y;
};

//rect类初始化方法
rect rect_init(rect self, int width, int height, char* color, void (* show)(shape), int position_x, int position_y)
{
    self.super_shape.width = width;
    self.super_shape.height = height;
    self.super_shape.color = color;
    self.super_shape.show = show;
    self.position_x = position_x;
    self.position_y = position_y;

    struct vtable vt = {shape_emptyfunc};
    self.super_shape.vpointer = &vt;

    return self;
}




//多态
//新建一个结构体A 里面是函数指针 让父结构体里有一个成员 这个成员是指向A的指针
//然后新建一个A类型结构体 里面放上子结构体自己的函数 让子结构体继承来的 指向A的指针 指向自己的A类结构体
//最后建立一个公共方法 这个公共方法的参数是一个父结构体类型的对象 在方法里通过参数里的"指向A的指针"成员来调用函数
//此时这个“指向A的指针”指向的是子结构体里的A类结构体 这也就实现了多态
//下面把“A类型结构体”放在了外面 其实也可以放在init方法里 但是那样的话就要加static关键字了
//其实上面的部分里 那个结构体A承担了“虚表”的角色 里面的函数指针承担了"虚函数"的角色

//其实如果看不懂上边的东西的话 也可以尝试另一种实现多态的方法 就是直接将父类的函数指针成员(这里就是上边的那个*show)取值为自己函数的地址
//在这里就是在执行poly_init和square_init时 将poly_dtfun和square_dtfun作为 void(* show)(shape)参数的值传进去
//然后直接 self.super_shape.show(self.super_shape); 即可
struct polyclass
{
    //继承shape
    shape super_shape;

    //poly类的属性
    int position_x;
    int position_y;
    char* points;
};

struct squareclass
{
    //继承shape
    shape super_shape;

    //square类的属性
    int position_x;
    int position_y;
    int sqr;
};

//poly的多态函数
void poly_dtfun(shape shape_super)
{
    printf("this is poly running dtfun\n");
}

//square的多态函数
void square_dtfun(shape shape_super)
{
    printf("this is square running dtfun\n");
}


struct vtable vtpoly = {poly_dtfun};  //将多态函数放入这个结构体 以便公共方法调用
struct vtable vtsquare = {square_dtfun};


//poly类的初始化函数
poly poly_init(poly self, int width, int height, char* color, void (* show)(shape), int position_x, int position_y, char* points)
{
    self.super_shape.width = width;
    self.super_shape.height = height;
    self.super_shape.color = color;
    self.super_shape.show = show;
    self.position_x = position_x;
    self.position_y = position_y;
    self.points = points;
    self.super_shape.vpointer = &vtpoly;

    return self;
}

//square类的初始化函数
square square_init(square self, int width, int height, char* color, void (* show)(shape), int position_x, int position_y)
{
    self.super_shape.width = width;
    self.super_shape.height = height;
    self.super_shape.color = color;
    self.super_shape.show = show;
    self.position_x = position_x;
    self.position_y = position_y;
    self.sqr = width * width;
    self.super_shape.vpointer = &vtsquare;

    return self;
}

//shape和poly的公共方法 也就是多态的调用同一方法 不同对象(类)有不同功能
void shape_poly_public_method(shape shape_super)
{
    shape_super.vpointer->dtfun(shape_super);
}



int main(void)
{
    // //测试封装
    // shape s; //新建s对象
    // s = shape_init(s, 100, 100, "red", shape_show);
    // s.show(s);


    // //测试继承
    // rect r; //新建r对象
    // r = rect_init(r, 200, 300, "blue", shape_show, 140, 180);
    // r.super_shape.show(r.super_shape);
    // printf("position_x: %d\n", r.position_x);
    // printf("position_y %d\n", r.position_y);


    // //测试多态
    // poly p;
    // square s;

    // p = poly_init(p, 200, 100, "volite", shape_show, 10, 20, "10,10 20,20 30,30 10,10");
    // s = square_init(s, 140, 120, "pink", shape_show, 234, 155);

    //实现多态的另一种方式
    // p.super_shape.show(p.super_shape);
    // s.super_shape.show(s.super_shape);

    // shape_poly_public_method(p.super_shape);
    // shape_poly_public_method(s.super_shape);


    return 0;
}