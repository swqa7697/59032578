using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using SocketHandler;

public class ClientHandler: MonoBehaviour
{
    public Rigidbody2D myBall;
    public Rigidbody2D player0;
    public Text text0;

	private void Awake()
    {
        Screen.SetResolution(1920, 1080, false);
	}
	
	private void FixedUpdate()
    {
		
	}
}
