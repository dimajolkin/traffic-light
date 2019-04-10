
boolean contains(String str, String substring);

class Input {
  protected:
    String cmd = "";
  public:
    Input(String cmd): cmd(cmd) {}

    boolean is(String name) {
      return contains(cmd, name + "=");
    }

    String get(String name) {
      return cmd.substring(cmd.indexOf("=") + 1);
    }
};


boolean contains(String str, String substring) {
  return str.indexOf(substring) != -1;
}
