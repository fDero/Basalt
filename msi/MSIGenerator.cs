using System;
using WixSharp;

namespace BasaltInstaller
{
    public class MSIGenerator
    {
        static void Main()
        {
            string exePath = @"..\out\bin\basalt.exe";
            string licenseRtfPath = @"..\EULA.rtf";
            string licenseTxtPath = @"..\LICENSE";
            string installDir = @"%ProgramFiles%\basalt\v0.0.0-alpha";
            var project = new Project("basalt-v0.0.0-alpha",
                new Dir(installDir, 
                    new File(exePath),
                    new File(licenseTxtPath),
                    new File(licenseRtfPath)
                ),
                new EnvironmentVariable("Path", @"[INSTALLDIR]")
                {
                    Id = "Path_INSTALLDIR",
                    Action = EnvVarAction.set,
                    Part = EnvVarPart.last,
                    Permanent = false,
                    System = true,
                }
            );
            project.LicenceFile = licenseRtfPath;
            project.GUID = new Guid("03d24f34-2adb-4bc9-8325-94a90aefdb11");
            project.BuildMsi();
        }
    }
}