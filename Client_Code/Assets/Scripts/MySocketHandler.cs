using System;
using System.Net;
using System.Net.Sockets;

namespace SocketHandler
{
    public class MySocketHandler
    {
        public static Socket mySocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        private static IPAddress myIP;
        private static int myPort;

        private static int tries = 0;
        private static string player0;

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

        
    }
}
