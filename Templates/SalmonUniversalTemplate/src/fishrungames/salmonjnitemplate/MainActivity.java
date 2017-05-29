package fishrungames.salmonjnitemplate;

import fishrungames.engine.EngineWrapper;

//Deprecated
//import fishrungames.androidjnitemplate.R;

import android.app.Activity;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Bundle;

import android.view.GestureDetector;
import android.view.KeyEvent;
import android.view.GestureDetector.SimpleOnGestureListener;
import android.view.MotionEvent;



public class MainActivity extends Activity
{

	GLView mView;
	
	@Override
	protected void onCreate(Bundle icicle)
	{
		
		super.onCreate(icicle);

		EngineWrapper.LoadSalmonEngineLibrary();

		EngineWrapper.SetActivityInstance(this);
		EngineWrapper.SetupEnviroment();
		
		String apkFilePath = null;
		ApplicationInfo appInfo = null;
		PackageManager packMgmr = this.getPackageManager();
		try {
		        appInfo = packMgmr.getApplicationInfo("fishrungames.salmonjnitemplate", 0);
		    } catch (NameNotFoundException e) {
		 
		 e.printStackTrace();
		throw new RuntimeException("Unable to locate assets, aborting...");
		    }
		apkFilePath = appInfo.sourceDir;
		
		EngineWrapper.SetupApkFilePath(apkFilePath);
		
		mView = new GLView(getApplication());
		
		setContentView(mView);
		

		EngineWrapper.SetView(mView);
		
	}

	@Override
	protected void onPause()
	{
		EngineWrapper.Destroy();
		super.onPause();
		mView.onPause();
	}

	@Override
	protected void onResume()
	{
		//Don't write anything here!
		super.onResume();
		mView.onResume();
	}

	@Override
	protected void onStop()
	{
		//Don't write anything here!
		super.onStop();
	}
	
	public boolean onKeyDown(int keyCode, KeyEvent event)
	{
		
		EngineWrapper.ProcessKeyDown(keyCode, event);
		return super.onKeyDown(keyCode, event);
	}

	public boolean onTouchEvent(MotionEvent event)
	{
		EngineWrapper.ProcessTouchEvent(event);
		return true;
	}

}