using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using SocketHandler;

public class ConnectBtn: MonoBehaviour
{
    public InputField inputName;
    public InputField inputIP;
    public InputField inputPort;
    public Text status;

    private bool debug = false;

    private void Awake()
    {
        Screen.SetResolution(1920, 1080, false);
    }

    public void Connect()
    {
        if (debug)
            SceneManager.LoadScene("Main", LoadSceneMode.Single);

        MySocketHandler.SetAddress(inputIP.text, int.Parse(inputPort.text));

        string s = MySocketHandler.Connect(inputName.text);
        if (s != "")
            status.text = s;
        else
            SceneManager.LoadScene("Main", LoadSceneMode.Single);
    }
}
