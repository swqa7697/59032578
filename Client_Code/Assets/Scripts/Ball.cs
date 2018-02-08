using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Ball: MonoBehaviour
{
    public float mySpeed;

    private Rigidbody2D myRigidbody;
    private Vector2 myDirection;

	private void Awake()
    {
        myRigidbody = GetComponent<Rigidbody2D>();
        myDirection = new Vector2(Random.Range(-1.0f, 1.0f), Random.Range(-1.0f, 1.0f));
        myDirection.Normalize();
	}

    private void FixedUpdate()
    {
        move();
    }

    private void OnTriggerEnter2D(Collider2D other)
    {
        if (other.gameObject.tag == "Top_bottom_edge")
            myDirection.y = -myDirection.y;
        if (other.gameObject.tag == "Left_right_edge")
            myDirection.x = -myDirection.x;
    }

    private void move()
    {
        Vector2 movement = myDirection * mySpeed * Time.deltaTime;
        myRigidbody.MovePosition(myRigidbody.position + movement);
    }
}
