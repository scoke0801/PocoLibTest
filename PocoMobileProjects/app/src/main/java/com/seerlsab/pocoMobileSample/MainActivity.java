package com.seerlsab.pocoMobileSample;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = MainActivity.class.getSimpleName();
    private static String[] PERMISSIONS_STORAGE = {
            Manifest.permission.READ_EXTERNAL_STORAGE,
            Manifest.permission.WRITE_EXTERNAL_STORAGE
    };

    private final int PERMISSIONS_REQUEST_RESULT = 1;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("PocoTestMain");
    }

    TextView tv;
    EditText et;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        {
            /////////////////////// 파일 쓰기 ///////////////////////
            String str = "1234";
            String chacheDirPath = getCacheDir().getAbsolutePath();///data/user/0/com.seerlsab.pocoMobileSample/cache
            String exCacheDirPath = getExternalCacheDir().getAbsolutePath();//storage/emulated/0/Android/data/com.seerlsab.pocoMobileSample/cache
            String indirPath = getFilesDir().getPath();///data/user/0/com.seerlsab.pocoMobileSample/files
// 파일 생성
            File saveFile = new File("/sdcard/Android/data/com.seerlslab.pocoMobileSample"); // 저장 경로
// 폴더 생성
            if(!saveFile.exists()){ // 폴더 없을 경우
                saveFile.mkdir(); // 폴더 생성
            }
            try {
                long now = System.currentTimeMillis(); // 현재시간 받아오기
                Date date = new Date(now); // Date 객체 생성
                SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
                String nowTime = sdf.format(date);

                BufferedWriter buf = new BufferedWriter(new FileWriter(saveFile+"/CarnumData.txt", true));
                buf.append(nowTime + " "); // 날짜 쓰기
                buf.append(str); // 파일 쓰기
                buf.newLine(); // 개행
                buf.close();
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        // Example of a call to a native method
        tv =  (TextView)findViewById(R.id.ResultText);
        tv.setText(stringFromJNI());
        et =  (EditText)findViewById((R.id.editText));

        isStorageReadPermissionGranted();
        isStorageWritePermissionGranted();
    }

    @Override
    protected void onDestroy(){
        super.onDestroy();
        // Free JDK data
        // if needed
    }
    public void onBtnSend(View view) {
        tv.setText("눌림 - Send Button");
    }

    public void onBtnReceive(View view) {
        tv.setText("눌림 - Receive Button");
    }

    public void onBtnLog(View view) {
        tv.setText("눌림 - Log Button");

        String logMsg = et.getText().toString();
        if(logMsg.length() != 0) {
            LogMessage(logMsg);
            et.setText("");
        }
    }

    public native String stringFromJNI();
    public native void SendMessage(String mess);
    public native String ReceiveMessage();
    public native void LogMessage(String logMsg);

    public  boolean isStorageWritePermissionGranted() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (checkSelfPermission(android.Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED) {
                Log.v(TAG,"Write Permission is granted");
                return true;
            } else {
                Log.v(TAG,"Write Permission is revoked");
                ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, PERMISSIONS_REQUEST_RESULT);
                return false;
            }
        }
        else { //permission is automatically granted on sdk<23 upon installation
            Log.v(TAG,"Permission is granted");
            return true;
        }
    }
    public  boolean isStorageReadPermissionGranted() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (checkSelfPermission(android.Manifest.permission.READ_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED) {
                Log.v(TAG,"Read Permission is granted");
                return true;
            } else {
                Log.v(TAG,"Read Permission is revoked");
                ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.READ_EXTERNAL_STORAGE}, PERMISSIONS_REQUEST_RESULT);
                return false;
            }
        }
        else { //permission is automatically granted on sdk<23 upon installation
            Log.v(TAG,"Permission is granted");
            return true;
        }
    }
}