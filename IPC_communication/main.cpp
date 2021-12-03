#include <QCoreApplication>
#include <nmsgpack.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString x;
    for (int i=1; i<argc; i++)
    {
        x.append(argv[i]);
    }

   if (x== "-s") //start as sender
   {
       nmsgpack messenger (nullptr);
       messenger.fillDataBuffer(2,false,"example");
       for (;;)
       {
           int r=messenger.sendBuffer();
           printf("result of send = %d \n",r);
           sleep(1);
       }
   }

   if (x== "-r") //start as receiver
   {
      nmsgpack messenger (nullptr, true);
      for (;;)
      {
          messenger.receiveBuffer();
          sleep(1);
      }

   }


    return a.exec();
}
