头文件更新,详细如下:
1. 更名为csj.json
2. 将 __Null__  函数更名为 __empty__
3. __upgrade__ 函数添加了接受 `map<std::string,std::string>` 的重载版本
4. 增加了CsmanJson的构造函数,可以使用文件名字符串(string或者C风格)构造,若文件打开失败则CsmanJson为空
5. 提供了== 和 = 运算符
6. 新增reboot函数,解决了单个CsmanJson对象只能一次使用的问题
   1. 使用方法
   2. 传递一个CsmanJson 或者Json::Value 对象,相对于使用赋值运算符
   3. 传递文件名字符串(string或者C风格)重新获取一个json
      1. 可选map或无序map在reboot时更改版本信息
重载了输入输出符
+ 文件输入>>
+ 输出到文件
+ 标准输出(std::cout)

构造函数(成员Json::Value 解析json的一个对象)
+ 默认构造
  + 空
+ 单参数对象

成员函数
+ 无报错,但会输出(标准输出)字符串错误信息
+ upgrade
  + 接受无序map
  + 直接更改对象的键值对
  + 需要调用write函数将改动输出到json去
+ null
  + 判断数据成员是否为空的函数
+ read
  + 从文件中读取json
  + bool(返回值类型)
  + 接受实参为ifstream
  + 需要手动开关文件
+ write
  + 输出到目标json
  + bool
  + 接受实参为ofstream
  + 需要手动开关文件
+ file_change
  + 参数
    + json文件路径
    + 更改的pair
  + bool
  + 一步到位改写pair

#### 构造函数

```cpp
  CsmanJson test;//直接声明,数据成员为空
  if(test.file_change("csman.json",Bri::Mp<std::string,
    std::string>("Version","1.0.0.1")))Bri::out(test,std::cout,"");

  CsmanJson test1(test);
  if(test1.Null())Bri::out("Null");

  CsmanJson test2;
  std::ifstream ifs;
  ifs.open("csman.json");
  ifs>>test2;
  if(!test2.Null())Bri::out(test2);
  ifs.close();
  ```


#### 复制构造函数
```json
  CsmanJson test1(test);
  if(test1.Null())Bri::out("Null");//查看是否为空
  ```

#### 改当前路径下的json的标签内容,如果成功更改则输出json内容(以csman.json为例)
```cpp
  CsmanJson test;
  if(test.file_change("csman.json",Bri::Mp<std::string,
    std::string>("BaseUrl","baidu.com")))Bri::out(test,std::cout,"");
  ```
##### 文件操作后的结果：
  changes from(原json文件)
```json
  {
     "BaseUrl" : "pornhub.com",
     "Platform" : "Linux_GCC_AMD64",
     "Version" : "1.0.0.1"
  }
  ```
    to(更新后json文件)
```json
  {
     "BaseUrl" : "baidu.com",
     "Platform" : "Linux_GCC_AMD64",
     "Version" : "1.0.0.1"
  }
  ```

#### 使用文件流写入数据
```cpp
  CsmanJson test2;
  std::ifstream ifs;
  ifs.open("csman.json");
  ifs>>test2;//test2.read(ifs);//等价形式
  if(!test2.Null())Bri::out(test2);
  ifs.close();
  ```

#### 更改json内容(下面两种方式)

 1. 直接调用file_change更改内容(单条)

```cpp
//如若需要发issue或者call我添加无序map接口或者initializer_list接口
CsmanJson test;
if(test.file_change("csman.json",Bri::Mp<std::string,std::string>("BaseUrl","baidu.com")))std::cout<<"成功";
```

2. 调用read write
```cpp
CsmanJson test;
std::ifstream ifs;
ifs.open("csman.json");
if(test.read(ifs)){//read会验证文件是否打开
    ifs.close();
    //to do
    /*可以手动更改信息，不过得准确
     *也可以调用upgrade更改信息
     */
    //演示upgrade更新信息
    std::unordered_map<std::string,std::string>info={/*infomation*/};//take as follows to make an example
    //std::unordered_map<std::string,std::string>info={Bri::Mp<std::string,std::string>("BaseUrl","baidu.com")};
    test.upgrade(info);
    std::ofstream ofs;
    ofs.open("csman.json");
    if(ofs){test.write(ofs);}
    ofs.close();
}
ifs.close();
```

  changes from(原json文件)

```json
{
   "BaseUrl" : "",
   "Platform" : "Linux_GCC_AMD64",
   "Version" : "1.0.0.1"
}
```

to(更新后json文件)

```json
{
   "BaseUrl" : "baidu.com",
   "Platform" : "Linux_GCC_AMD64",
   "Version" : "1.0.0.1"
}
```

#### namespace内容两个输出函数,类似于py的print,不过第二个参数是输出流对象,第三个参数才是类似于py print 的end,但只接受C风格字符串
```cpp
static void out(const std::string& str,std::ostream&os=std::cout,const char*end="\n")；
template<typename T>
    static void out(const T& str,std::ostream&os=std::cout,const char*end="\n");
```
>tips：可以不管大写的Um和Mp,大写的与小写的差不多,都是模板别名(不过还是推荐小写,那才是真正的别名,大写的是自己写的),um为无序map的别名,mp为make_pair的别名
