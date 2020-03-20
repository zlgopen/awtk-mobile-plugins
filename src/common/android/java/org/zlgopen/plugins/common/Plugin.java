package org.zlgopen.plugins;

import java.lang.*;

public interface Plugin {
  public void deinit();
  public String run(String action, String args);
}
