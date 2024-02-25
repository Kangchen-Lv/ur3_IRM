#include<ros/ros.h>
#include<serial/serial.h>
#include<std_msgs/String.h>

#include<iostream>
#include<string>
#include<sstream>

using namespace std;


int serial_write(serial::Serial &ser, std::string &serial_msg)
{
    ser.write(serial_msg);
    return 0;
}


int serial_read(serial::Serial &ser, std::string &result)
{
    result = ser.read( ser.available() );
    return 0;
}


int main(int argc, char** argv)
{
    ros::init(argc, argv,"sucker_serial_port");
    ros::NodeHandle n;

   serial::Serial ser;

    //初始化串口相关设置
   ser.setPort("/dev/ttyUSB0");         //设置打开的串口名称
   ser.setBaudrate(115200);                //设置串口的波特率
   serial::Timeout to = serial::Timeout::simpleTimeout(1000);           //创建timeout
   ser.setTimeout(to);                           //设置串口的timeout

    //打开串口
    try
    {
        ser.open();         //打开串口
    }
    catch(const std::exception& e)
    {
        ROS_ERROR_STREAM("Unable to open port ");           //打开串口失败，打印信息
        return -1;
    }

    //判断串口是否成功打开
    if( ser.isOpen() )
    { 
        ROS_INFO_STREAM("Serial Port initialized. \n");         //成功打开串口，打印信息
    }
    else
    {
        return -1;
    }


    ros::Rate loop_rate(50);
	
	//data 为发送数据
    //result 为接收数据
  	std::string result;
    int func(0);
    
    cout << "Please input function number:" << endl;

    unsigned char close_data[8]={0x01,0x06,0x00,0x02,0x00,0x01,0xE9,0xCA};
    unsigned char open_data[8]={0x01,0x06,0x00,0x02,0x00,0x02,0xA9,0xCB};
    // unsigned char data[8];
    while( ros::ok() )
    {
      	//从键盘中读取键入数据
      	cout << "Your function number is: ";
        cin >> func;
        
  /*****************************************************************************
 * 以下逻辑可以按照你自己的写，主要工作是根据键盘键入的数据，为 data 赋值
 *****************************************************************************/
     	 switch (func)
        {
            case 0:     ser.write(close_data, 8);         break;
            case 1:     ser.write(open_data, 8);        break;
            default:    ROS_ERROR_STREAM("No this function number!!!");     break;
        }

        //串口写数据
        // ser.write(data);
        // cout << " the data write to serial is :  " << data.c_str();
        //串口读数据
        serial_read(ser, result);
        cout << " the data read from serial is : " << result.c_str();
        cout << endl;
    }

	ser.close();
    return 0;
}

