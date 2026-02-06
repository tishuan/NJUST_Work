import ohos.app.MainActivity;
import ohos.hiviewdfx.HiLogLabel;
import ohos.hiviewdfx.HiLog;
import java.io.BufferedReader;
import java.io.InputStreamReader;

public class PythonExecutor extends MainActivity {
    private static final HiLogLabel LABEL = new HiLogLabel(3, 0x00201, "PythonExecutor");

    public void executePythonScript() {
        try {
            ProcessBuilder pb = new ProcessBuilder("python", "../py/netService.py");
            Process process = pb.start();

            BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
            String line;
            while ((line = reader.readLine()) != null) {
                HiLog.info(LABEL, line);
            }
        } catch (Exception e) {
            HiLog.error(LABEL, "Error executing Python script: " + e.getMessage());
        }
    }
}