﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using SocketHandler;

public class Player0: MonoBehaviour
{
    public float mySpeed, xMin, xMax;

    private Rigidbody2D myRigidbody;
    private Vector2 myDirection;

	private void Awake()
    {
        myRigidbody = GetComponent<Rigidbody2D>();
	}
	
	private void FixedUpdate()
    {
        Move();
	}

    private void Move()
    {
        Vector2 input = Vector2.zero;
        if (Input.GetKey(KeyCode.LeftArrow))
            input += Vector2.left;
        if (Input.GetKey(KeyCode.RightArrow))
            input += Vector2.right;

        input.Normalize();

        Vector2 newPosition = myRigidbody.position + input * mySpeed * Time.deltaTime;
        newPosition = new Vector2(Mathf.Clamp(newPosition.x, xMin, xMax), -4.8f);

        myRigidbody.MovePosition(newPosition);

        //Send the position to Server
        newPosition = Camera.main.WorldToScreenPoint(newPosition);
        MySocketHandler.Send("P0 " + Mathf.RoundToInt(newPosition.x));
    }
}
