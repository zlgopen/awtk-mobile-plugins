package org.zlgopen.plugins;

import java.lang.*;
import android.content.Intent;

public interface Plugin {
  public void deinit();
  public String run(String action, String args);
  public boolean matchRequest(int requestCode);
  public void onActivityResult(int requestCode, int resultCode, Intent data);
}
