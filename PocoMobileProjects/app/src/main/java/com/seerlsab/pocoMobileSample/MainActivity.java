package com.seerlsab.pocoMobileSample;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    TextView tv;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        tv = findViewById(R.id.ResultText);
        tv.setText(stringFromJNI());
    }


    public void onBtnSend(View view) {
        tv.setText("Send Button");
    }
    public void onBtnReceive(View view) {
        tv.setText("Receive Button");
    }
    public void onBtnLog(View view) {
        tv.setText("Log Button");
    }

    public native String stringFromJNI();
}