using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class BallTester: MonoBehaviour
{
    public float mySpeed;
    
    public Text text;

    private Rigidbody2D myRigidbody;
    private Vector2 position;
    //private int score0;

    private void Awake()
    {
        myRigidbody = GetComponent<Rigidbody2D>();
    }

    private void FixedUpdate()
    {
        position = Camera.main.WorldToScreenPoint(new Vector2(0f,-4.8f));
        text.text = "X: " + position.x + "\nY: " + Mathf.Round(position.y);

        //Move();
    }

    private void Update()
    {
        Vector2 pos = new Vector2(0f, -4.8f);
        if (Input.GetMouseButtonDown(0))
            myRigidbody.MovePosition(pos);
    }

    private void Move()
    {
        Vector2 input = Vector2.zero;
        if (Input.GetKey(KeyCode.LeftArrow))
            input += Vector2.left;
        if (Input.GetKey(KeyCode.RightArrow))
            input += Vector2.right;
        if (Input.GetKey(KeyCode.UpArrow))
            input += Vector2.up;
        if (Input.GetKey(KeyCode.DownArrow))
            input += Vector2.down;

        input.Normalize();

        Vector2 newPosition = myRigidbody.position + input * mySpeed * Time.deltaTime;
        myRigidbody.MovePosition(newPosition);
    }
}
