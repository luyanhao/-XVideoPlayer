package com.lyhao.xvideoplayer

import android.os.Bundle
import android.widget.TextView
import androidx.activity.ComponentActivity
import com.lyhao.xvideoplayer.media.FFMediaPlayer

class MainActivity2 : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.main_activity)
        findViewById<TextView>(R.id.tv_ffmpeg_version).text = FFMediaPlayer.getFFmpegVersion();
    }
}