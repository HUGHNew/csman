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
