#ifndef LOADINGANIMA_H
#define LOADINGANIMA_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QPropertyAnimation>
#include <QDebug>

class loadingAnima : public QObject
{
    Q_OBJECT
public:
    explicit loadingAnima(QObject *parent = nullptr);

signals:

};

#define Abs(x) ((x)>=0?(x):-(x))

class RoundProgressBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(float _value READ getValue WRITE _setValue)
public:
    enum SwitchFlags    //控制变量
    {
        defaultText =0x00000001,        //默认文字
        linearColor =0x00000004,        //内圈渐变色
        decorateDot =0x00000008,        //装饰圆点
        outterCirle =0x0000000e,        //外圈
        animation   =0x00000010,        //动画
        all         =0xffffffff        //显示所有效果
    };
    enum InnerDefaultTextStyle    //内部文字展示样式
    {
        percent     =0x00000001,        //百分比
        value       =0x00000002,        //值
        valueAndMax =0x00000004        //值与最大值
    };

    explicit RoundProgressBar(QWidget *parent = 0);
    ~RoundProgressBar();

    void setdefault(int,bool);//设置初始角度，顺时针逆时针
    void setOutterBarWidth(float);//设置外圈宽度
    void setInnerBarWidth(float);//设置内圈宽度
    void setRange(float, float);//设置范围
    void setValue(float);//设置当前值
    void setOutterColor(const QColor&);//设置外圈颜色
    void setInnerColor(const QColor&,const QColor&);//设置内圈渐变色
    void setInnerColor(const QColor&);
    void setDefaultTextColor(const QColor&);//设置默认文字颜色
    void setControlFlags(int);//设置控制命令
    void setPrecision(int); //设置显示数字精度
    //设置内圈默认文字样式
    inline void setInnerDefaultTextStyle(InnerDefaultTextStyle style){_innerDefaultTextStyle=style;}
    //获取当前值
    inline float getValue(){return _value;}

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *event);

private:
    int _startAngle;//起始角度 范围0-360
    bool _clockWise;//顺时针
    float _outterBarWidth;//outterBar宽度
    float _innerBarWidth;//innerBar宽度
    float _dotX;//圆点矩形坐标
    float _dotY;
    float _min;//最小值,最大值，当前值
    float _max;
    float _value;
    QColor _outterColor;//外圈颜色
    QColor _startColor;//内圈渐变颜色
    QColor _endColor;
    QColor _textColor;//默认文字颜色
    int _precision; //小数点精度
    float _squareStart; //绘制正方形信息
    float _squareWidth;
    quint32 _controlFlags=0x00000000;//控制信号
    InnerDefaultTextStyle _innerDefaultTextStyle; //内圈默认文字样式

    void paintOutterBar(QPainter &);//绘制外圈
    void paintInnerBar(QPainter &);//绘制内圈
    void paintDot(QPainter &);//绘制装饰圆点
    void paintText(QPainter &);//绘制默认内置文字
    void _setValue(float); //设置当前值
    void caculateSquare(); //计算绘制正方形区域信息

};

#endif // LOADINGANIMA_H
