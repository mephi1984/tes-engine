package fishrungames.wallpapertemplate;

import fishrungames.wallpapertemplate.JniWrapper;
import fishrungames.wallpapertemplate.R;


import android.content.Intent;

import android.net.Uri;
import android.os.Bundle;
import android.preference.CheckBoxPreference;
import android.preference.ListPreference;
import android.preference.Preference;
import android.preference.PreferenceActivity;

import android.preference.Preference.OnPreferenceChangeListener;


public class Prefs extends PreferenceActivity implements
		OnPreferenceChangeListener {

	
	private CheckBoxPreference redBkgPref;
	

	public static Uri currImageURI;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.prefliste);
		addPreferencesFromResource(R.xml.preferences);
		
		redBkgPref = (CheckBoxPreference)this.getPreferenceManager().findPreference("RedBackground");
		
		if (redBkgPref.getKey().compareToIgnoreCase("RedBackground") == 0) {
			if (!redBkgPref.isChecked())
			{
				JniWrapper.SetRedBkgPref(false);
			}
			else
			{
				JniWrapper.SetRedBkgPref(true);
			}
		}	

	}


	@Override
	public boolean onPreferenceChange(Preference preference, Object newValue) {
		
		return true;

	}


	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		
	}

}
