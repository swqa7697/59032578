using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace SocketHandler
{
    public class MySocketHandler
    {
        private static Socket mySocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        private static IPAddress myIP;
        private static int myPort;

        private static string player0;
        private static int tries = 0;

        public static void SetAddress(string ip, int port)
        {
            myIP = IPAddress.Parse(ip);
            myPort = port;
        }

        public static string Connect(string playerName)
        {
            IPEndPoint point = new IPEndPoint(myIP, myPort);
            try
            {
                mySocket.Connect(point);
                tries = 0;
                player0 = playerName;
                return "";
            }
            catch
            {
                return "Connection Failed (" + tries++ + ") Please try again...";
            }
        }

        public static void Send(string data)
        {
            mySocket.Send(Encoding.ASCII.GetBytes(data));
        }

        public static string Receive()
        {
            return "pass";
        }

        public static string GetID0()
        {
            return player0;
        }
    }
}
