#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <iostream>
#include <cmath>
#include <QChar>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit->setReadOnly(true);//只读模式
    ui->lineEdit->move(QPoint(0,0));//设置lineEdit位置
    ui->lineEdit->setAlignment(Qt::AlignRight);//从右侧显示
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::getHex(int i)
{
    QByteArray Hex;
    i++;
    for(;i<ba.size();i++)
    {
        if((ba[i]>='0'&&ba[i]<='9')||(ba[i]>='A'&&ba[i]<='F'))
        {
            char x=ba[i];
            Hex.append(x);
        }
        else break;
    }
    int trans=Hex.toInt(nullptr,16);
    out.append(QString::number(trans));
    i--;
    return i;
}

int MainWindow::getBin(int i)
{
    QByteArray Bin;
    i++;
    for(;i<ba.size();i++)
    {
        if(ba[i]=='0'||ba[i]=='1')
        {
            char x=ba[i];
            Bin.append(x);
        }
        else break;
    }
    int trans=Bin.toInt(nullptr,2);
    out.append(QString::number(trans));
    i--;
    return i;
}

void MainWindow::getChar()
{
    ba=in.toLatin1();
}

void MainWindow::getSuffix()
{
  for(int i=0;i<ba.size();i++)
  {
      if((ba[i]>='0' && ba[i]<='9')||ba[i]=='.')//1,数字直接输出
      {
         QString str;
         QByteArray m;
         for(;(ba[i]>='0' && ba[i]<='9')||ba[i]=='.';i++)
         {
             //将一个数字存储到字节数组m中
             char x=ba[i];
             m.append(x);
         }
         //将字节数组m转换为QString，并将字符串输出
         str=QString(m);
         out.append(str);
         i--;
      }
      else if(ba[i]=='x')
          i=getHex(i);
      else if(ba[i]=='b')
          i=getBin(i);
      else if(s1.isEmpty())//2,遇见非数字时, 如果堆栈为空，则直接把该字符放入堆栈
      {
          if(ba[i]=='x')
              i=getHex(i);
          else if(ba[i]=='b')
              i=getBin(i);
          else
              s1.push(ba[i]);
          if(ba[i]=='s'||ba[i]=='c'||ba[i]=='t'||ba[i]=='l')
              i=i+2;
      }
      else if(ba[i]=='+' || ba[i]=='-')
          //2.1,如果是+或- 那么遍历堆栈栈顶元素 一直输出优先级>=加减的 即输出栈顶的+-*/,
          //当遇到其他字符时（如左括号）或堆栈空时break
      {
          while(!s1.empty()){
              if(s1.top()=='+' || s1.top()=='-'||s1.top()=='*' || s1.top()=='/'||s1.top()=='s'||s1.top()=='c'||s1.top()=='t'||s1.top()=='^'||s1.top()=='l'){
                  //把字符转换为字符串输出
                  QString str=QString(s1.pop());
                  out.append(str);
               }else break;
          }
          s1.push(ba[i]);
      }
      else if(ba[i]=='*' || ba[i]=='/')
          //2.2,如果是*或/ 那么遍历堆栈栈顶元素 一直输出优先级>=乘除的 即输出栈顶的*/,
          //当遇到其他字符时（如左括号，减号，除号）或堆栈空时break
      {
          while(!s1.empty()){
              if(s1.top()=='*' || s1.top()=='/'||s1.top()=='s'||s1.top()=='c'||s1.top()=='t'||s1.top()=='^'||s1.top()=='l'){
                  //把字符转换为字符串输出
                  QString str=QString(s1.pop());
                  out.append(str);
               }else break;
          }
          s1.push(ba[i]);
      }
      else if(ba[i]=='s'||ba[i]=='c'||ba[i]=='t')
      {
          while(!s1.empty()){
              if(s1.top()=='s'||s1.top()=='c'||s1.top()=='t'||s1.top()=='^'||s1.top()=='l'){
                  //把字符转换为字符串输出
                  QString str=QString(s1.pop());
                  out.append(str);
               }else break;
          }
          s1.push(ba[i]);
      }
      else if(ba[i]=='^')
      {
          while(!s1.empty()){
              if(s1.top()=='^'){
                   //把字符转换为字符串输出
                   QString str=QString(s1.pop());
                   out.append(str);
               }else break;
          }
          s1.push(ba[i]);
      }
      else if(ba[i]==')')
          //2.3,如果该字符是右括号时，一直输出栈顶元素，直到遇见左括号为止break:
      {
          while(!s1.empty()){
              if(s1.top()=='('){
                  s1.pop();
                  break;
              }
              //把字符转换为字符串输出
              QString str=QString(s1.pop());
              out.append(str);
          }
      }
      else if(ba[i]=='(')
          //2.4,如果该字符是左括号时，直接放入堆栈
      {
          s1.push(ba[i]);
      }
  }
  //字符串循环结束后 再输出堆栈中剩余的数据
  while(!s1.empty()){
      //把字符转换为字符串输出
      QString str=QString(s1.pop());
      out.append(str);
  }
  //输出后缀表达式
  qDebug()<<out;
}

void MainWindow::Calc_Suffix()
{
  for(int i=0;i<out.size();i++)
  {
      bool is_Num =true;
      out[i].toDouble(&is_Num);
      if(is_Num)//如果是数字，直接入栈
      {
          s2.push(out[i].toDouble(&is_Num));
      }
      else if(out[i]=='s'||out[i]=='c'||out[i]=='t'||out[i]=='^')
      {
          double x=s2.pop();
          if(out[i]=='s')
              x=sin(x);
          else if(out[i]=='c')
              x=cos(x);
          else if(out[i]=='t')
              x=tan(x);
          else if(out[i]=='l')
              x=log(x);
          else if(out[i]=='^')
          {
              int y=x;
              x=s2.pop();
              x=pow(x,y);
          }
          s2.push(x);
      }
      else if(out[i]=="+"||out[i]=="-"||out[i]=="*"||out[i]=="/")//如果是运算符，弹栈两次，并进行运算，运算后，结果入栈
      {
          double x=s2.pop();
          double y=s2.pop();
          if(out[i]=="+")
          {
              s2.push(y+x);
          }else if(out[i]=="-"){
              s2.push(y-x);
          }else if(out[i]=="*"){
              s2.push(y*x);
          }else if(out[i]=="/"){
              s2.push(y/x);
          }
      }
  }
  result = s2.pop();
  qDebug()<<result;
}

void MainWindow::m_init()
{
    ba.clear();
    out.clear();
}

void MainWindow::on_pushButton_clicked()
{
    in=ui->lineEdit->text();//获取表达式
    getChar();
    getSuffix();
    Calc_Suffix();
    ui->lineEdit->clear();//清空文本编辑框
    ui->lineEdit->setText(QString("%1%2").arg('=').arg(result));//显示结果
    m_init();
}

void MainWindow::on_pushButton_2_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(0));
}

void MainWindow::on_pushButton_3_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(1));
}

void MainWindow::on_pushButton_4_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(2));
}

void MainWindow::on_pushButton_5_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(3));
}

void MainWindow::on_pushButton_6_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(4));
}

void MainWindow::on_pushButton_7_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(5));
}

void MainWindow::on_pushButton_8_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(6));
}

void MainWindow::on_pushButton_9_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(7));
}

void MainWindow::on_pushButton_10_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(8));
}

void MainWindow::on_pushButton_11_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(9));
}

void MainWindow::on_pushButton_12_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("+"));
}

void MainWindow::on_pushButton_13_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("-"));
}

void MainWindow::on_pushButton_14_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("*"));
}

void MainWindow::on_pushButton_15_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("/"));
}

void MainWindow::on_pushButton_20_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("."));
}

void MainWindow::on_pushButton_18_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("("));
}

void MainWindow::on_pushButton_19_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(")"));
}

void MainWindow::on_pushButton_16_clicked()
{
    //清空所有内容，并初始化
    ui->lineEdit->clear();
    m_init();
}

void MainWindow::on_pushButton_17_clicked()
{
    //退格删除一个字符
    ui->lineEdit->backspace();
}

void MainWindow::on_pushButton_21_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("sin"));
}

void MainWindow::on_pushButton_22_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("cos"));
}

void MainWindow::on_pushButton_23_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("tan"));
}

void MainWindow::on_pushButton_24_clicked()//最小公倍数，c_mul
{
    QString str=ui->lineEdit->text();
    QString l1="0",l2="0";
    QByteArray yue = str.toLatin1();
    bool hasSpace = false;
    QString c_div="1";
    int max_yue;
    for(int i=0;i<yue.size();i++)
    {
         char x=yue[i];
         if(x=='.')
            hasSpace=true;
         else if(x!='.'&&hasSpace)
            l2+=QString(x);
         else
            l1+=QString(x);
    }
    int s1=l1.toInt();
    int s2=l2.toInt();
    int max;
    if(s1>s2)
        max=s1;
    else
        max=s2;
    for(int j=2;j<max;j++)
    {
        if(s1 % j==0&&s2%j==0)
      {
            max_yue=j;
      }
    }
    int z=s1*s2/max_yue;
    QString c_mul=QString::number(z);
    ui->lineEdit->setText(c_mul);
}

void MainWindow::on_pushButton_25_clicked()//公约数,两个数以小数点分格
{
    QString str=ui->lineEdit->text();
    QString l1="0",l2="0";
    QByteArray yue = str.toLatin1();
    bool hasSpace = false;
    QString c_div="1";
    for(int i=0;i<yue.size();i++)
    {
         char x=yue[i];
         if(x=='.')
            hasSpace=true;
         else if(x!='.'&&hasSpace)
            l2+=QString(x);
         else
            l1+=QString(x);
    }
    int s1=l1.toInt();
    int s2=l2.toInt();
    int max;
    if(s1>s2)
        max=s1;
    else
        max=s2;
    for(int j=2;j<max;j++)
    {
        if(s1 % j==0&&s2%j==0)
      {
            c_div+="*";
            c_div+=QString::number(j);
      }
    }
    ui->lineEdit->setText(c_div);
}

void MainWindow::on_pushButton_26_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("^"));
}

void MainWindow::on_pushButton_27_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("log"));
}

void MainWindow::on_pushButton_28_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("b"));
}

void MainWindow::on_pushButton_29_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("x"));
}

void MainWindow::on_pushButton_30_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("A"));
}

void MainWindow::on_pushButton_31_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("B"));
}

void MainWindow::on_pushButton_32_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("C"));
}

void MainWindow::on_pushButton_33_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("D"));
}

void MainWindow::on_pushButton_34_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("E"));
}

void MainWindow::on_pushButton_35_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("F"));
}

void MainWindow::on_pushButton_36_clicked()
{
    QString str=ui->lineEdit->text();
    QByteArray l1,l2;
    l1.append('0');
    l2.append('0');
    QByteArray com = str.toLatin1();
    bool hasSpace = false;
    bool is_Hex1 = false;
    bool is_Bin1 = false;
    bool is_Hex2 = false;
    bool is_Bin2 = false;
    for(int i=0;i<com.size();i++)
    {
        char x=com[i];
        if(x=='.')
           hasSpace=true;
        else if(x!='.'&&hasSpace)
        {
           if(x=='x')
           {
               is_Hex2 = true;
           }
           else if(x=='b')
           {
               is_Bin2 = true;
           }
           else
               l2.append(x);
        }
        else
        {
            if(x=='x')
            {
                is_Hex1 = true;
            }
            else if(x=='b')
            {
                is_Bin1 = true;
            }
            else
                l1.append(x);
        }
    }

    int s1;
    int s2;

    if(is_Hex1)
        s1 = l1.toInt(nullptr,16);
    else if(is_Bin1)
        s1 = l1.toInt(nullptr,2);
    else
        s1 = l1.toInt();

    if(is_Hex2)
        s2 = l2.toInt(nullptr,16);
    else if(is_Bin2)
        s2 = l2.toInt(nullptr,2);
    else
        s2 = l2.toInt();

    qDebug() << s1 << " "  <<s2;

    if(s1>s2)
        ui->lineEdit->setText(QString("%1%2").arg(str).arg("大于"));
    else if(s1==s2)
        ui->lineEdit->setText(QString("%1%2").arg(str).arg("等于"));
    else
        ui->lineEdit->setText(QString("%1%2").arg(str).arg("小于"));
}


