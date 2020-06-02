package com.example.hello_vmex;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Handler;
import android.telecom.Call;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.example.hello_vmex.jni.CallbackInterface;
import com.example.hello_vmex.jni.NativeWrapper;

import org.w3c.dom.Text;

public class MainActivity extends AppCompatActivity implements
        Button.OnClickListener
{
    
    class Callback implements CallbackInterface {
        double generator_t_total = 0;
        double regressor_t_total = 0;
        double gaze_t_total = 0;
        double t_total = 0;
    
        long total_timestamp = System.currentTimeMillis();
    
        long fps_timestamp = System.currentTimeMillis();
        int fps_counter = 0;
        final int fps_renew_ms = 500;
    
        int counter = 0;
    
        @Override
        public void updateGazeInferenceTime(final float ms) {
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    final long timestamp = System.currentTimeMillis();
                
                    String elapsed = floatToString(ms, 5) + " ms";
                    TextView textView = findViewById(R.id.gaze_t);
                    textView.setText(elapsed);
                
                    if ( t_total > 2000) {
                        gaze_t_total += ms;
                        counter += 1;
                        elapsed = floatToString((float)(gaze_t_total / counter), 5)+" ms";
                        ((TextView)findViewById(R.id.gaze_t_avg)).setText(elapsed);
                    }
                
                    // Latency (including multithread callbacks)
                    elapsed = floatToString(timestamp - total_timestamp, 5) + " ms";
                    ((TextView)findViewById(R.id.total_t)).setText(elapsed);
                
                    // fps
                    if (timestamp > fps_timestamp + fps_renew_ms) {
                        String fps = floatToString(
                                (float)(1000.0 * fps_counter / (timestamp - fps_timestamp)), 5) + " fps";
                        ((TextView)findViewById(R.id.total_fps)).setText(fps);
                    
                        fps_counter = 0;
                        fps_timestamp = timestamp;
                    }
                    
                    fps_counter += 1;
                
                    t_total += timestamp - total_timestamp;
                    total_timestamp = System.currentTimeMillis();
                }
            });
        }
    
        @Override
        public void updateGeneratorInferenceTime(final float ms) {
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                
                    TextView textView = findViewById(R.id.generator_t);
                    String res = floatToString(ms, 5) + " ms";
                    textView.setText(res);
                
                    if (t_total < 2000) return;
                    generator_t_total += ms;
                    res = floatToString((float)(generator_t_total / counter), 5) + " ms";
                    TextView textView_avg = findViewById(R.id.generator_t_avg);
                    textView_avg.setText(res);
                }
            });
        }
    
        @Override
        public void updateRegressorInferenceTime(final float ms) {
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                
                    String elapsed = floatToString(ms, 5) + " ms";
                    TextView textView = findViewById(R.id.regressor_t);
                    textView.setText(elapsed);
                
                    // Average inference time
                    if (t_total < 2000) return;
                    regressor_t_total += ms;
                    elapsed = floatToString((float)(regressor_t_total / counter), 5) + " ms";
                    TextView textView_avg = findViewById(R.id.regressor_t_avg);
                    textView_avg.setText(elapsed);
                }
            });
        }
    
        @Override
        public void setText(String string) {
            TextView textView = findViewById(R.id.output);
            textView.setText(string);
        }
    
        @Override
        public void appendText(String string) {
            try {
                TextView text = findViewById(R.id.output);
                text.append(string);
            } catch (IndexOutOfBoundsException e){
                e.printStackTrace();
                ((TextView)findViewById(R.id.textView)).setText(e.toString());
            }

//            String str = text.getText().toString();
//            str = str.concat(string);
//            text.setText(str);
        }
    
        @Override
        public void clearText() {
            try {
                TextView text = findViewById(R.id.output);
                text.setText("");
            } catch (IndexOutOfBoundsException e){
                e.printStackTrace();
                ((TextView)findViewById(R.id.textView)).setText(e.toString());
            }
        }
        
    
        String floatToString(float f, int digit){
            String str = String.valueOf(f);
            return str.substring(0, Math.min(str.length(), digit));
        }
    
        void resetTimeInfo(){
            generator_t_total = 0;
            regressor_t_total = 0;
            gaze_t_total = 0;
            t_total = 0;
        
            total_timestamp = System.currentTimeMillis();
        
            fps_timestamp = System.currentTimeMillis();
            fps_counter = 0;
        
            counter = 0;
            
            ((TextView)findViewById(R.id.generator_t)).setText("Collecting...");
            ((TextView)findViewById(R.id.generator_t_avg)).setText("Collecting...");
            ((TextView)findViewById(R.id.regressor_t)).setText("Collecting...");
            ((TextView)findViewById(R.id.regressor_t_avg)).setText("Collecting...");
            ((TextView)findViewById(R.id.gaze_t)).setText("Collecting...");
            ((TextView)findViewById(R.id.gaze_t_avg)).setText("Collecting...");
            ((TextView)findViewById(R.id.total_t)).setText("Collecting...");
            ((TextView)findViewById(R.id.total_fps)).setText("Collecting...");
        }
    }
    
    Callback callback;
    NativeWrapper wrapper;
    
    int numThread = 2;
    boolean stopMainThread = false;
    
    Button run, stop;
    Button runMain, stopMain;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        callback = new Callback();
        wrapper = new NativeWrapper();
        wrapper.setCallback(callback);
        wrapper.setNumThreads(numThread);
        
        run = findViewById(R.id.button_run);
        stop = findViewById(R.id.button_stop);
        runMain = findViewById(R.id.button_run_main);
        stopMain = findViewById(R.id.button_stop_main);
        
        run.setOnClickListener(this);
        stop.setOnClickListener(this);
        runMain.setOnClickListener(this);
        stopMain.setOnClickListener(this);
        
        stop.setEnabled(false);
        stopMain.setEnabled(false);
    }
    
    @Override
    public void onClick(View view){
        switch(view.getId()) {
                
            case R.id.button_run:
                runMain.setEnabled(false);
                run.setEnabled(false);
                stop.setEnabled(true);
                
                callback.resetTimeInfo();
                wrapper.run();
                break;
                
            case R.id.button_stop:
                wrapper.stop();
                
                runMain.setEnabled(true);
                run.setEnabled(true);
                stop.setEnabled(false);
                break;
                
            case R.id.button_run_main:
                runMain.setEnabled(false);
                run.setEnabled(false);
                stopMain.setEnabled(true);
                stopMainThread = false;
                
                callback.resetTimeInfo();
                
                final Handler handler = new Handler();
                handler.post(new Runnable() {
                    @Override
                    public void run() {
                        if(stopMainThread) {
                            callback.setText("(Main Thread) Stop");
                            return;
                        }
                        
                        wrapper.runMain();
                        handler.postDelayed(this, 1);
                    }
                });
                break;
                
            case R.id.button_stop_main:
                run.setEnabled(true);
                runMain.setEnabled(true);
                stopMain.setEnabled(false);
                
                stopMainThread = true;
                break;
        }
    }
    
    @Override
    protected void onStop() {
        super.onStop();
        
        wrapper.stop();
    }
}
