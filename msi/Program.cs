﻿using System;
using WixSharp;

namespace BasaltInstaller
{
    public class Program
    {
        static void Main()
        {
            string exePath = @"..\out\bin\basalt.exe";
            var project = new Project("basalt-v0.0.0-alpha",
                new Dir(@"%ProgramFiles%\basalt\v0.0.0-alpha",
                    new File(exePath)
                )
            );
            project.GUID = new Guid("03d24f34-2adb-4bc9-8325-94a90aefdb11");
            project.BuildMsi();
        }
    }
}