# ZMQ_Load_Balancer
Software for files transfer control for FC

***How to use***

***ZMQ_Load_Balancer***

Для управления маршрутизацией разработан класс ***ZMQ_Load_Balancer*** 
Что бы настроить маршрутизатор, необходимо указать порты, к которым будет установлено подключение, и через которе будет проходить маршрутизируемый трафик.

Пример задания соединения

        #include <Core/Core.h>
        #include "LoadBalancer.h"
        
        using namespace Upp;
    
        CONSOLE_APP_MAIN
        {
          //Создадим экземпляр маршрутизатора
          ZMQ_Load_Balancer loadBalancer;
          
          //Установить соединение
          //Параметры:
          //  1) Upp::String Frontend_Bind_String
          //  2) Upp::String Backend_Bind_String
          loadBalancer.Bind("tcp://*:5555", "tcp://*:5556");
          
          //Перейти в рабочий режим
          loadBalancer.DoWork();
        }
