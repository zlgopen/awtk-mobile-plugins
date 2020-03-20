package org.zlgopen.plugins;

import java.lang.*;
import android.content.Intent;

public interface Plugin {
  public void deinit();
  public boolean matchRequest(int requestCode);
  public boolean run(String action, String callerInfo, String args);
  public void onActivityResult(int requestCode, int resultCode, Intent data);
}
