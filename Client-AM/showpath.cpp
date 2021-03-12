#include "showpath.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Showpath::Showpath(QWidget *parent) : QWidget(parent)
{
    qwtPlot = new QwtPlot(this);
    //qwtPlot->move(0,0);
    qwtPlot->setAxisScale(QwtPlot::yLeft,-200,200,40);
    qwtPlot->setAxisScale(QwtPlot::xBottom,-200,200,40);

    //使用滚轮放大/缩小
   (void) new QwtPlotMagnifier(qwtPlot->canvas());
   //使用鼠标左键平移
   (void) new QwtPlotPanner(qwtPlot->canvas());
}

void Showpath::plotlayer(QString input)
{

    qwtPlot->detachItems();
    curve_sum1.clear();
    qwtPlot->replot();
    QByteArray ba = input.toLatin1(); // must
    const char *input_filename = ba.data();
    infile.open(input_filename);
    std::string line;
    int count = 0;
    int count2 = 0;
    int pos=0;
    int k=0, x, y;
    std::string temp_x, temp_y;

    while(getline(infile, line))
    {
      if(line=="##")
     {
        count++;
        if(count%2==0)
        {
          count2 = 0;
          layer.push_back(polygons);
          polygons.clear();
         }
     }else if(line=="#")
     {
        count2++;
        if(count2%2==0)
        {
           //tmp_part.outline = temp_polygon;
        }else{
           polygons.push_back(Path());
        }
      }else{
        int line_size = line.size();
        for(int i=0; i<line_size; i++)
        {
           if(line[i]=='(')
           {
              pos = i+1;
           }else if(line[i]==','&& line[i-1]!=')')
           {
              k=i;
              temp_x = line.substr(pos,k-pos);
              std::istringstream digital(temp_x);
              digital >> x;
              pos = i+1;
            }else if(line[i]==')')
            {
               k = i;
               temp_y = line.substr(pos,k-pos);
               std::istringstream digital(temp_y);
               digital >> y;
               polygons[count2/2].push_back(Point(x,y));
            }
          }
       }
    }
    polygons.clear();
    infile.close();
    QVector<QVector<QPointF>> vector;
    int layersize = layer.size();
    for(int i=0; i<layersize; i++)
   {
      int polygonsize = layer[i].size();
      vector.append(QVector<QPointF>());
      for(int j=0; j<polygonsize; j++)
     {
        int polysize = layer[i][j].size();
        for(int z=0; z<polysize; z++)
       {
            QPointF point;
            point.setX(layer[i][j][z].X/1000.0);
            point.setY(layer[i][j][z].Y/1000.0);
            vector[i].append(point);
       }
     }
   }
    layer.clear();
    QVector<QwtPointSeriesData*> series_sum;
    int vectorsize = vector.size();
    for(int i=0; i<vectorsize; i++)
    {
        QwtPointSeriesData* series = new QwtPointSeriesData(vector[i]);
        series_sum.append(series);
    }
    //QVector<QwtPlotCurve*> curve_sum;
    //create plot item
    for(int i=0; i<vectorsize; i++)
    {
        curve_sum1.append(new QwtPlotCurve());
        //QwtPlotCurve* curve = new QwtPlotCurve("Curve");
        curve_sum1[i]->setData(series_sum[i]);
        curve_sum1[i]->attach(qwtPlot);
    }
    qwtPlot->replot();
}

void Showpath::plotpath(QString input)
{
    //qwtPlot->detachItems();
    curve_sum2.clear();
    qwtPlot->replot();
    QByteArray ba = input.toLatin1(); // must
    const char *input_filename = ba.data();
    infile.open(input_filename);
    std::string line;
    int pos=0;
    int k=0, x, y;
    std::string temp_x, temp_y;

    while(getline(infile, line))
    {
        polygons.push_back(Path());
        int line_size = line.size();
        for(int i=0; i<line_size; i++)
        {
           if(line[i]=='(')
           {
              pos = i+1;
           }else if(line[i]==','&& line[i-1]!=')')
           {
              k=i;
              temp_x = line.substr(pos,k-pos);
              std::istringstream digital(temp_x);
              digital >> x;
              pos = i+1;
            }else if(line[i]==')')
            {
               k = i;
               temp_y = line.substr(pos,k-pos);
               std::istringstream digital(temp_y);
               digital >> y;
               polygons.back().push_back(Point(x,y));
            }
         }
    }
    layer_path.push_back(polygons);
    infile.close();

    QVector<QVector<QPointF>> vector;
    int layerpathsize = layer_path.size();
    for(int i=0; i<layerpathsize; i++)
   {
      int polygonsize = layer_path[i].size();
      for(int j=0; j<polygonsize; j++)
     {
        vector.append(QVector<QPointF>());
        int polysize = layer_path[i][j].size();
        for(int z=0; z<polysize; z++)
       {
            QPointF point;
            point.setX(layer_path[i][j][z].X/1000.0);
            point.setY(layer_path[i][j][z].Y/1000.0);
            vector[j].append(point);
       }
     }
   }
    layer_path.clear();
    //构造曲线数据
    QVector<QwtPointSeriesData*> series_sum;
    int vectorsize = vector.size();
    for(int i=0; i<vectorsize; i++)
    {
        QwtPointSeriesData* series = new QwtPointSeriesData(vector[i]);
        series_sum.append(series);
    }

    //QVector<QwtPlotCurve*> curve_sum;
    //create plot item
    for(int i=0; i<vectorsize; i++)
    {
        curve_sum2.append(new QwtPlotCurve());
        curve_sum2[i]->setData(series_sum[i]);
        curve_sum2[i]->attach(qwtPlot);
    }

    qwtPlot->replot();
}

void Showpath::clearpath()
{
    qwtPlot->detachItems();
    curve_sum1.clear();
    curve_sum2.clear();
    layer.clear();
    layer_path.clear();
    polygons.clear();
    qwtPlot->replot();
    infile.close();
}
