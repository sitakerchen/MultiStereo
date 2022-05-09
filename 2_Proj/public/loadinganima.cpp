#include "loadinganima.h"

loadingAnima::loadingAnima(QObject *parent)
    : QObject{parent}
{

}

RoundProgressBar::RoundProgressBar(QWidget *parent) :
    QWidget(parent),
    _min(0),
    _max(100),
    _value(0),
    _precision(0)
{
    setdefault(90,true);//设置初始角度，顺时针逆时针
    setOutterBarWidth(18);//设置默认外圈宽度
    setInnerBarWidth(16);//设置默认内圈宽度
    setRange(0,100);//设置默认范围
    setValue(75);//设置默认值
    setOutterColor(QColor(233,248,248));//设置外圈颜色
    setInnerColor(QColor(49, 177, 190),QColor(133, 243, 244)); //设置默认渐变色
    setDefaultTextColor(QColor(49,177,190));//设置默认文字颜色
    setPrecision(0);//设置默认精度
    setInnerDefaultTextStyle(RoundProgressBar::percent);//设置内圈默认文字样式
}

RoundProgressBar::~RoundProgressBar()
{

}

void RoundProgressBar::setdefault(int startAngle,bool clockWise)//设置初始角度，顺时针逆时针
{
    _startAngle=startAngle;
    _clockWise=clockWise;
}
void RoundProgressBar::setOutterBarWidth(float width)//设置外圈宽度
{
    _outterBarWidth=width;
}
void RoundProgressBar::setInnerBarWidth(float width)//设置内圈宽度
{
    _innerBarWidth=width;
}

void RoundProgressBar::setRange(float min,float max)//设置值的范围
{
    if(max<min)    //todo 和value比较
    {
        max=100; min=0;
    }
    else
    {
        _max=max; _min=min;
    }
}

void RoundProgressBar::setValue(float value)//设置当前值
{
    QPropertyAnimation* animation=new QPropertyAnimation(this,"_value");
    animation->setDuration(500);
    animation->setStartValue(_value);
    animation->setEndValue(value);
    animation->setEasingCurve(QEasingCurve::OutQuad);
    animation->start();
}

void RoundProgressBar::_setValue(float value)
{
    _value=value;
    repaint();
}
void RoundProgressBar::setOutterColor(const QColor& outterColor)//设置外圈颜色
{
    _outterColor=outterColor;
}
void RoundProgressBar::setInnerColor(const QColor& startColor,const QColor& endColor)//设置内圈渐变色
{
    _startColor=startColor;
    _endColor=endColor;
}
void RoundProgressBar::setInnerColor(const QColor& startColor)//设置内圈渐变色
{
    _startColor=startColor;
}
void RoundProgressBar::setDefaultTextColor(const QColor& textColor)
{
    _textColor=textColor;
}
void RoundProgressBar::setControlFlags(int flags)//设置控制
{
    this->_controlFlags|=flags;
}
void RoundProgressBar::setPrecision(int precision)//设置显示数字精度
{
    _precision=precision;
}

//**********************************************************************************
void RoundProgressBar::resizeEvent(QResizeEvent *event)
{
    if(_outterBarWidth>_innerBarWidth)    //根据内外圈宽度设置控件最小大小
        this->setMinimumSize(_outterBarWidth*8,_outterBarWidth*8);
    else
        this->setMinimumSize(_innerBarWidth*8,_innerBarWidth*8);
    caculateSquare();    //计算绘制正方形区域信息
}

void RoundProgressBar::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    paintOutterBar(painter);    //绘制外圈
    paintInnerBar(painter);    //绘制内圈
    paintDot(painter);    //绘制外圈
    paintText(painter);    //绘制文字
}

void RoundProgressBar::caculateSquare()//计算绘制正方形区域信息
{
    int minWidth=qMin(this->width(),this->height());
    float barWidth=qMax(_outterBarWidth,_innerBarWidth);
    _squareWidth=minWidth-barWidth-2;
    _squareStart=barWidth/2+1;
    _dotX=_squareStart+_squareWidth/2;
    _dotY=_squareStart;
}

void RoundProgressBar::paintOutterBar(QPainter &painter)//绘制外圈
{
    if(!(_controlFlags&outterCirle))
        return;
    QPen pen;
    pen.setWidth(_outterBarWidth);
    pen.setColor(_outterColor);
    painter.setPen(pen);
    QRectF rectangle(_squareStart,_squareStart,_squareWidth,_squareWidth);

    painter.drawEllipse(rectangle);    //从90度开始，逆时针旋转
}

void RoundProgressBar::paintInnerBar(QPainter& painter)//绘制内圈
{
    QPen pen;
    if(!(_controlFlags&linearColor))
        pen.setColor(_startColor);
    else
    {
        QLinearGradient gradient(0, 0, 0, _squareWidth);
        gradient.setColorAt(0, _startColor);
        gradient.setColorAt(1, _endColor);
        QBrush brush(gradient);
        pen.setBrush(brush);
    }
    pen.setWidth(_innerBarWidth);
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);
    QRectF rectangle(_squareStart,_squareStart,_squareWidth,_squareWidth);
    //从90度开始，逆时针旋转
    int startAngle=_startAngle*16;
    int spanAngle=(_value-_min)/(_max-_min)*360*16*(_clockWise?-1:1);
    painter.drawArc(rectangle,startAngle,spanAngle);
}

void RoundProgressBar::paintDot(QPainter& painter)//绘制装饰圆点
{
    if(!(_controlFlags&decorateDot))
        return;
    if(_innerBarWidth<3)    //当bar宽度小于3时，便不再绘制装饰圆点
        return;
    painter.setPen(QColor(255,255,255));
    painter.setBrush(QColor(255,255,255));
    //区域为圆点绘制正方形区域
    painter.drawEllipse(_dotX-_innerBarWidth/6,_dotY-_innerBarWidth/6,_innerBarWidth/3,_innerBarWidth/3);
}

void RoundProgressBar::paintText(QPainter& painter)//绘制默认内置文字
{
    if(!(_controlFlags&defaultText))
        return;
    painter.setPen(_textColor);
    painter.setFont(QFont("Microsoft YaHei",22,75));
    switch (_innerDefaultTextStyle) {
    case value:
        painter.drawText(_squareStart,_squareStart,_squareWidth,_squareWidth,Qt::AlignCenter,QString::number(_value,'f',_precision));
        break;
    case valueAndMax:
        painter.drawText(_squareStart,_squareStart,_squareWidth,_squareWidth,Qt::AlignCenter,
                         QString::number(_value,'f',_precision)+"/"+QString::number(_max,'f',_precision));
        break;
    case percent:
        painter.drawText(_squareStart,_squareStart,_squareWidth,_squareWidth,Qt::AlignCenter,
                         QString::number(_value/_max*100,'f',_precision)+"%");
        break;
    default:
        break;
    }
}
