package com.lyhao.xvideoplayer;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import androidx.appcompat.app.AppCompatActivity;
import com.lyhao.xvideoplayer.media.FFMediaPlayer;
import com.lyhao.xvideoplayer.ui.NativeMediaPlayerActivity;
import com.lyhao.xvideoplayer.util.CommonUtils;

public class MainActivity2 extends AppCompatActivity implements View.OnClickListener {

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main_activity);
        ((TextView)findViewById(R.id.tv_ffmpeg_version)).setText(FFMediaPlayer.getFFmpegVersion());

    }

    @Override
    public void onResume() {
        super.onResume();
        CommonUtils.copyAssetsDirToSDCard(this, "byteflow", "/sdcard");
    }

    @Override
    public void onClick(View view) {
        if (view.getId() == R.id.action_NativeMediaPlayerActivity) {
            startActivity(new Intent(this, NativeMediaPlayerActivity.class));
        }
    }


}