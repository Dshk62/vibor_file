#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStringListModel>
#include <QDebug>
//#include <iostream>
using namespace std ;

int poisk_null(QString s)
{
    int k = 0;
    int n = s.length();
    int i;
    for (i = 0; i < n; i++)
    {
        if (s[i] == '0')
            k++;
    }
    return k;
}

int cifra(QString s)
{
    int n = s.length();
    QString str = "0123456789";
    for (int i = 0; i < n; i++)
    {
        if (!str.contains(s[i]))
            return 0;
    }
    return 1;
}
QString findslovo(QString str)
{
    QString rez_st="", slovo, seps(",:.!? "), ms = "", pms = "";
    int n = str.length();
    if (!seps.contains(str[n - 1]))
        str = str + " ";

    while (seps.contains(str[0]))
    {
        str.remove(0, 1);
    }

    int i = 0, mx = 0, k = 0, pmx;
    while (str != "")
    {
        if (seps.contains(str[i]))
        {
            slovo = str.left(i);
            str.remove(0, i + 1);
            i = 0;
            if (cifra(slovo))
            {
                k = poisk_null(slovo);
                if (k >= mx)
                {
                    mx=k;
                    rez_st=slovo;
                }
            }
        }
        else
            i++;
    }
    return rez_st;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    qDebug() << "Начало";
    QString f;
    if (ui->checkBox->isChecked())
        f= "in1.txt";
    if (ui->checkBox_2->isChecked())
        f= "in2.txt";
    QFile fin(f);
    QFile fout("output.txt");
    // Создание модели данных, если она еще не создана
    if (!ui->listView->model()) {
        ui->listView->setModel(new QStringListModel(this));
    }

    // Получение указателя на модель данных
    QStringListModel* model = qobject_cast<QStringListModel*>(ui->listView->model());


    fin.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!fin.isOpen())
    {
        // Добавление сообщения об ошибке в модель данных
        QString errorMesage = "Файл не удалось открыть";
        QStringList stringList = model->stringList();
        stringList.append(errorMesage);
        model->setStringList(stringList);
        return;
    }
    if (fin.atEnd())
    {
        // Добавление сообщения об ошибке в модель данных
        QString errorMessage = "Файл пустой";
        QStringList stringListt = model->stringList();
        stringListt.append(errorMessage);
        model->setStringList(stringListt);
        return;
    }
    QString rez_st="", slovo, seps(",:.!? "), ms = "", pms = "";
    while (!fin.atEnd())
    {
        QString str = fin.readLine();
        QTextStream out(&fout);
        out << str;

        int n = str.length();
        if (!seps.contains(str[n - 1]))
            str = str + " ";

        while (seps.contains(str[0]))
        {
            str.remove(0, 1);
        }

        int i = 0, mx = 0, k = 0, pmx;
        while (str != "")
        {
            if (seps.contains(str[i]))
            {
                slovo = str.left(i);
                str.remove(0, i + 1);
                i = 0;
                if (cifra(slovo))
                {
                    k = poisk_null(slovo);
                    if (k >= mx)
                    {
                        pmx = mx;
                        mx = k;
                        pms = ms;
                        ms = slovo;
                    }
                }
            }
            else
                i++;
        }
        if (mx == pmx)
        {
            rez_st = rez_st + pms + " ";
        }
        else
            rez_st = rez_st + ms + " ";
    }

    QString finil_rez=findslovo(rez_st);

    QString Message = finil_rez;
    QStringList strList = model->stringList();
    strList.append(Message);
    model->setStringList(strList);


    fin.close();

}
