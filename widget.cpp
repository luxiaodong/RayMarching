#include "widget.h"
#include <QPainter>
#include "graymarching.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent) , m_hasDraw(false)
{
    this->resize(600,600);
}

Widget::~Widget()
{

}

void Widget::paintEvent(QPaintEvent*)
{
    if(m_hasDraw) return ;
    this->render();
}

void Widget::render()
{
    m_hasDraw = true;
    QSize size = this->size();
    QPainter painter(this);
    GRayMarching rayMarching(size);
    rayMarching.draw();
    painter.drawImage(QRectF(QPoint(0,0),size), rayMarching.m_image);
}
