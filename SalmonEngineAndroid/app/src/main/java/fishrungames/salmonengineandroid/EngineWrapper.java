package fishrungames.salmonengineandroid;


import java.io.IOException;
import java.io.InputStream;
import android.content.Context;
import android.content.ContextWrapper;
import android.content.res.AssetFileDescriptor;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.SoundPool;
import android.opengl.GLSurfaceView;
import android.view.GestureDetector;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.GestureDetector.SimpleOnGestureListener;
//import android.net.Uri;

import java.util.Collections;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Map;

public class EngineWrapper {
	
	static class CustomGestureListener extends SimpleOnGestureListener
	{
			
		@Override
		public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX,
					float velocityY)
		{
			EngineWrapper.OnFling(velocityX, velocityY, e2.getEventTime());

			return true;
		}

	}
	
	static class JniDestroyRunnable implements Runnable
	{
		public void run()
		{
			EngineWrapper.Destroy();
		}
	}

	static ContextWrapper Instance = null;
	
	static Map<String, Integer> FileMap = new HashMap<String, Integer>();
	
	static Map<String, Integer> SoundMap = new HashMap<String, Integer>();
	
	static Context context;
	
	static SoundPool soundPool;
	
	static MediaPlayer mp;
	
	static GLSurfaceView mView;
	
	static GestureDetector customGestureDetector  = new GestureDetector(new CustomGestureListener());

	
	public static void SetActivityInstance(ContextWrapper inst)
	{
		Instance = inst;
		context = Instance.getBaseContext();
		
		soundPool = new SoundPool(4, AudioManager.STREAM_MUSIC, 0);
		
		mp = new MediaPlayer();
		
	}
	
	public static void SetView(GLSurfaceView view)
	{
		
		mView = view;
	
	}
	
	public static String GetApplicationDir()
	{
		return Instance.getFilesDir().getPath();
	}
	
	public static void AddToFileMap(String s, Integer i)
	{
		FileMap.put(s, i);
		
	}
	
	
	
	public synchronized static int OpenFile(String fileName)
	{
		String newFileName;
		
		int i = fileName.lastIndexOf(".");
		
		if (i == -1)
		{
			
			newFileName = fileName;
		}
		else
		{
			newFileName = fileName.substring(0, i);
		}

		
		if (!FileMap.containsKey(newFileName))
		{
			return 0;
		}
		
        Integer id = FileMap.get(newFileName);
        
        try 
        {
        	
        	InputStream is = Instance.getResources().openRawResource(id);

        	byte[] buffer = new byte[256];
        	int bytesRead;
        	
        	CreateFile(is.available());

        	while ((bytesRead = is.read(buffer, 0, 256)) != -1)
        	{
        		WriteToFile(buffer, bytesRead);
        	}
        	
        	
		} catch (Throwable t) {
			ConsoleOut("Exception in FileWrapper::OpenFile!\n");
			return 0;
			
		}
        
        return 1;
        
	}
	
	public static native void SetupEnviroment();
	
	public static native void SetupApkFilePath(String s);
	
	public static native void ConsoleOut(String s);
	
	public static native void CreateFile(int fileSize);
	
	public static native void WriteToFile(byte [] buffer, int bufferSize);
	
	
	public static void LoadSound(String soundFileName)
	{
		
		if (SoundMap.containsKey(soundFileName))
		{
			return;
		}
		/*
		String newFileName;
		
		int i = soundFileName.lastIndexOf(".");
		
		if (i == -1)
		{
			
			newFileName = soundFileName;
		}
		else
		{
			newFileName = soundFileName.substring(0, i);
		}*/
		
		//int soundId = soundPool.load(context, FileMap.get(newFileName), 1);
		
		AssetFileDescriptor afd = null;
		
		try
		{
			afd = Instance.getAssets().openFd(soundFileName);
		} catch (IOException e)
		{
			e.printStackTrace();
		}
		
		int soundId = soundPool.load(afd, 1);
		
		SoundMap.put(soundFileName, soundId);

	}
	
	public static void ReleaseAllSoundsAndMusic()
	{
		
		mp.stop();
		mp.reset();
		
		//soundPool.autoPause();
		
		Enumeration<Integer> i = Collections.enumeration(SoundMap.values());
		
		while(i.hasMoreElements())
		{
			soundPool.stop(i.nextElement());
		}
		
		SoundMap.clear();
		
	}
	
	public static void PauseAllSoundsAndMusic()
	{
		//NIU right now
		
		soundPool.autoPause();

		mp.pause();
	}
	
	public static void ResumeAllSoundsAndMusic()
	{
		
		//NIU right now
	
		soundPool.autoResume();
		
		mp.start();
	}
		
	
	public static void StopMusic()
	{
		mp.stop();
		mp.reset();
	}
	
	public static void PlayMusic(String musicFileName)
	{
		InnerPlayMusic(musicFileName, false);
	}
	
	
	public static void PlayMusicLooped(String musicFileName)
	{
		InnerPlayMusic(musicFileName, true);
	}
	
	private static void InnerPlayMusic(String musicFileName, boolean looped)
	{
		/*
		String newFileName;
		
		int i = musicFileName.lastIndexOf(".");
		
		if (i == -1)
		{
			
			newFileName = musicFileName;
		}
		else
		{
			newFileName = musicFileName.substring(0, i);
		}
		*/
		try
		{
			//Uri path = Uri.parse("android.resource://"+Instance.getPackageName()+"/assets/"+newFileName);
			
			AssetFileDescriptor afd = null;
			
			try
			{
				afd = Instance.getAssets().openFd(musicFileName);
			} catch (IOException e)
			{
				e.printStackTrace();
			}
			
			//mp.setDataSource(context, path);
			
			mp.setDataSource(afd.getFileDescriptor(),afd.getStartOffset(),afd.getLength());
			
			mp.prepare();
			mp.setLooping(looped);
			mp.start();
		} catch (IllegalArgumentException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IllegalStateException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		
	}
	
	public static void PlaySound(String soundFileName)
	{
		if (!SoundMap.containsKey(soundFileName))
		{
			return;
		}
		
		int soundId = SoundMap.get(soundFileName);
		
		soundPool.play(soundId, 1,1,1,0,1);
	}
	
	
	public static void LoadSalmonEngineLibrary()
	{
		System.loadLibrary("engine");
	}
	
	static 
	{
		//System.loadLibrary("gnustl_shared");
    }
	
	//Application methods
	
	public static native void Update(long dt);
	  
    public static native void Destroy();
    public static native int IsInited();
    public static native void OnTapDown(float x, float y, long time);
    public static native void OnTapUp(float x, float y, long time);
    public static native void OnTapUpAfterMove(float x, float y, long time);
    public static native void OnTapMove(float x, float y, long time);
    
    public static native void OnFling(float velocityX, float velocityY, long time);
    public static native void OnScroll(float distanceX, float distanceY, long time);
    
    public static native void OnKeyPress(int keyCode);
    
    
    public static void CallDestroy()
    {
    	mView.queueEvent(new JniDestroyRunnable());
    }
    
    
    
    
    public static void ProcessKeyDown(int keyCode, KeyEvent event)
	{
		int ascii_keycode = keyCode;
			
		if (keyCode == KeyEvent.KEYCODE_DEL)
		{
			ascii_keycode = 8; //Hack - getUnicodeChar does not recognize backspace
		}
		else
		{
			ascii_keycode = event.getUnicodeChar();
		}
		
		OnKeyPress(ascii_keycode);
	
	}
    
    static boolean IsScrolling = false;
    static float PrevMovePosX = 0;
    static float PrevMovePosY = 0; 
    static boolean EnableFlingGesture = false;
    
    public static void ProcessTouchEvent(MotionEvent event)
	{
    	
    	if (EnableFlingGesture)
    	{
    		if (customGestureDetector.onTouchEvent(event))
    		{
    			return;
    		}
    	}
		
		
		if (event.getAction() == MotionEvent.ACTION_MOVE)
		{
			float x = event.getX();
			float y = (float) mView.getHeight() - event.getY();
			
			float oldX = PrevMovePosX;
			float oldY = (float) mView.getHeight() - PrevMovePosY;
			
		
			float shiftX = x - oldX;
			float shiftY = y - oldY;
			
			if (Math.abs(shiftX) > 0.001f || Math.abs(shiftY) > 0.001f)
			{
				OnScroll(-shiftX, -shiftY, event.getEventTime());
			}
			
			PrevMovePosX = event.getX();
			PrevMovePosY = event.getY();
			
			IsScrolling = true;
			
		}
		


		if (event.getAction() == MotionEvent.ACTION_UP)
		{
			
			
			float x = event.getX();
			float y = (float) mView.getHeight() - event.getY();

			if (IsScrolling)
			{
				IsScrolling = false;
				OnTapUpAfterMove(x, y, event.getEventTime());
			}
			else
			{
				OnTapUp(x, y, event.getEventTime());
			}

		}
		
		if (event.getAction() == MotionEvent.ACTION_DOWN)
		{
			float x = event.getX();
			float y = (float) mView.getHeight() - event.getY();
			
			PrevMovePosX = event.getX();
			PrevMovePosY = event.getY();
		
			OnTapDown(x, y, event.getEventTime());

		}
	}
    
}