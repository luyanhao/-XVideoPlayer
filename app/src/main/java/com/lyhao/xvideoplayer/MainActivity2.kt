package com.lyhao.xvideoplayer

import android.content.Intent
import android.os.Bundle
import android.view.View
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import com.lyhao.xvideoplayer.media.FFMediaPlayer
import com.lyhao.xvideoplayer.ui.NativeMediaPlayerActivity

class MainActivity2 : AppCompatActivity(), View.OnClickListener {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.main_activity)
        findViewById<TextView>(R.id.tv_ffmpeg_version).text = FFMediaPlayer.getFFmpegVersion();

    }

    override fun onClick(view: View?) {
        when(view?.id) {
            R.id.action_NativeMediaPlayerActivity -> {
                startActivity(Intent(this@MainActivity2, NativeMediaPlayerActivity::class.java))
            }
        }
    }


}