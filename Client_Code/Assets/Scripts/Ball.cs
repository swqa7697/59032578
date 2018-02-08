using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using SocketHandler;

public class Ball: MonoBehaviour
{
    public float mySpeed; //will use server

    private Rigidbody2D myRigidbody;
    private Vector2 myDirection; //server

	private void Awake()
    {
        myRigidbody = GetComponent<Rigidbody2D>(); //server
        myDirection = new Vector2(Random.Range(-1.0f, 1.0f), Random.Range(-1.0f, 1.0f)); //server
        myDirection.Normalize();
	}

    private void FixedUpdate()
    {
        Move();
    }

    private void OnTriggerEnter2D(Collider2D other)
    {
        if (other.gameObject.tag == "Top_bottom_edge")
            myDirection.y = -myDirection.y; //will call server
        if (other.gameObject.tag == "Left_right_edge")
            myDirection.x = -myDirection.x; //will call server
    }

    private void Move()
    {
        Vector2 movement = myDirection * mySpeed * Time.deltaTime; //server
        myRigidbody.MovePosition(myRigidbody.position + movement);
    }
}
