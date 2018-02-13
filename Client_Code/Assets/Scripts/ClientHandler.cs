using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using SocketHandler;

public class ClientHandler: MonoBehaviour
{
    public Rigidbody2D myBall;
    public Text text0;

    private Vector2 ballPosition;
    private int score0;

	private void Awake()
    {
        Screen.SetResolution(1920, 1080, false);
        SendID();
	}
	
	private void FixedUpdate()
    {
        //Receive data
        string data = MySocketHandler.Receive();

        //Resolve data
        string[] lines = data.Trim().Split('\n');
        foreach(string line in lines)
        {
            string[] sub = line.Split();

            if (sub[0] == "BP")
                ballPosition = new Vector2(float.Parse(sub[1]), float.Parse(sub[2]));
            else if (sub[0] == "S0")
                score0 = int.Parse(sub[1]);
        }

        //Process data
        MoveBall();
        UpdateScore();
    }

    private void SendID()
    {
        MySocketHandler.Send("N0 "+MySocketHandler.GetID0());
    }

    private void MoveBall()
    {
        myBall.MovePosition(ballPosition);
    }

    private void UpdateScore()
    {
        text0.text = MySocketHandler.GetID0() + "'s\nScore: " + score0;
    }
}
