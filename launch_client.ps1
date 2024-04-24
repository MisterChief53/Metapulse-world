# Copyright (c) Contributors to the Open 3D Engine Project.
# For complete copyright and license terms please see the LICENSE at the root of this distribution.
# SPDX-License-Identifier: Apache-2.0 OR MIT

# Continuous Integration CLI entrypoint script to start CTest, triggering post-build tests

$AWS_EC2_METADATA_DISABLED = $true

# Start the game launcher in a separate process
Start-Process -FilePath .\build\windows\bin\profile\metapulseWorld.GameLauncher.exe -ArgumentList '--console-command-file=launch_client.cfg', '-rhi=vulkan'

# Continuously download the image in a loop
while ($true) {
  curl -o Assets/img.png http://localhost:8080/image/get
  Start-Sleep -Seconds 5
}