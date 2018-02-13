using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using SocketHandler;

public class Ball: MonoBehaviour
{
/*
    private Rigidbody2D myRigidbody;

	private void Awake()
    {
        myRigidbody = GetComponent<Rigidbody2D>();
	}
*/
    private void OnTriggerEnter2D(Collider2D other)
    {
    /*
        if (other.gameObject.tag == "Top_bottom_edge")
            myDirection.y = -myDirection.y; //will call server
        if (other.gameObject.tag == "Left_right_edge")
            myDirection.x = -myDirection.x; //will call server
    */
        if (other.gameObject.tag == "ThisPlayer")
        {
            //Call server to change direction
            MySocketHandler.Send("DC 0");
        }
    }
}
