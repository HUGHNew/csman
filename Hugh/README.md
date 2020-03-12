命名空间`Csj`中有一个异常类`LogicError`用来抛出错误的路径(或者说是错误的Key)
`Csj::Package`拥有三个数据成员:储存json的成员和、同类型的指针和一个用于判断指针是否启用的 bool 值,非`Csj::Package`类型的复制都用指针完成***(即类内部为指针行为)***

<font size=4>正常使用情况下,这个头文件的东西不会给 warning 只可能有抛异常,那多半是路径写错了</font>

<font size=4>内部的指针从第一次使用`operator[]`调用存在`Key`后开始工作</font>


构造函数
```cpp
Package(){}
Package(const Package&p)
```
读写类
```cpp
//----------写-------------
write()//第一个参数为输出流,第二可选参数为可选参数(即输出对象)默认为存储成员,可用Inner()函数改为输出指针所指对象
write(std::cout);//将储存的json输出
write(std::cout,PACKAGE.Inner());//输出指针所指对象
//---------读----------
read(ifs)//ifs 为文件输入流
read(PACKAGE a)//用另一个package类对象初始化
//当然重定向符(<< >>)都可以用
operator[]//string 或 const char* 使用前需要知道value是否存在
//或者用来创造对象 行为类似于 map
//改值自己小心使用

operator=//使用来改值
//
```

<font size=4 color=#ff0000>目录下沉(进去对象内部)</font>

```cpp
//Pac中储存的 json 为 Generic.json
Pac.Enter("Generic/cics.csbuild/Version");//必须使用路径形式 且为相对路径 (第一次可理解为 指针在根目录)
//Pac.Inner() 为
/*
{
    "1.0": {
        "Dependencies": {
            "cs.runtime": "Stable",
            "cs.develop": "Stable"
        },
        "ContentUrl": "http://mirrors.covariant.cn/csman/Generic/cics.csbuild_1.0.zip"
    }
}
*/
```
此时可以直接修改该对象(Version 内部)


<font size=4 color=#ff0000>内部指针控制</font>

```cpp
changePoint(std::string& path)//使用绝对路径改变内部指针指向

reset()//将内部指针置为nullptr

Inner()//返回内部指针指向的 json value 部分 如果指针为 nullptr 则返回当前存储的整个 json 对象
```

<font size=4 color=#ff0000>增减控制</font>


```cpp
ArrayMake(string Key,int index,可变参数)//增加 array index 为参数开始的下标, 若不从 0 开始 则前面用 null 补齐
//也可以用于改值
erase(string Key) //只要存在的东西随便删,只直接当前指针所指的对象中的 Key 只有一个参数 Key string 类型,支持相对路径行为
//别问我其他重载和第二个参数是什么 什么都不是,什么都没有
```
<font size=4 color=#ff0000>成员相关</font>

```cpp
isMember()//扔个string 或者 字符串都行 判断是不是存在的对象


GetMemberNames();

GetArrayNames();
//上面两个的返回值都是如下一行的这个东西  看函数名字就知道是干嘛的了
std::vector<std::string>
```

<font size=4 color=#ff0000>其他控制</font>

```cpp
showInner()//返回 inner 的值

empty() // 内部是否为空


operator bool()//类类型转换函数  可用于判断 是否为空
```
