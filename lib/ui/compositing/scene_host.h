// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_LIB_UI_COMPOSITING_SCENE_HOST_H_
#define FLUTTER_LIB_UI_COMPOSITING_SCENE_HOST_H_

#include <lib/ui/scenic/cpp/id.h>
#include <zircon/types.h>

#include <cstdint>

#include "dart-pkg/zircon/sdk_ext/handle.h"
#include "flutter/fml/memory/ref_counted.h"
#include "flutter/fml/memory/weak_ptr.h"
#include "flutter/fml/task_runner.h"
#include "flutter/lib/ui/dart_wrapper.h"
#include "third_party/tonic/dart_library_natives.h"
#include "third_party/tonic/dart_persistent_value.h"

namespace flutter {

class SceneHost : public RefCountedDartWrappable<SceneHost> {
  DEFINE_WRAPPERTYPEINFO();
  FML_FRIEND_MAKE_REF_COUNTED(SceneHost);

 public:
  static void RegisterNatives(tonic::DartLibraryNatives* natives);
  static fml::RefPtr<SceneHost> Create(
      fml::RefPtr<zircon::dart::Handle> viewHolderToken,
      Dart_Handle viewConnectedCallback,
      Dart_Handle viewDisconnectedCallback,
      Dart_Handle viewStateChangedCallback);
  static void OnViewConnected(scenic::ResourceId id);
  static void OnViewDisconnected(scenic::ResourceId id);
  static void OnViewStateChanged(scenic::ResourceId id, bool state);

  ~SceneHost() override;

  zx_koid_t id() const { return koid_; }

  // These are visible to Dart.
  void dispose();
  void setProperties(double width,
                     double height,
                     double insetTop,
                     double insetRight,
                     double insetBottom,
                     double insetLeft,
                     bool focusable);

 private:
  SceneHost(fml::RefPtr<zircon::dart::Handle> viewHolderToken,
            Dart_Handle viewConnectedCallback,
            Dart_Handle viewDisconnectedCallback,
            Dart_Handle viewStateChangedCallback);

  fml::RefPtr<fml::TaskRunner> raster_task_runner_;
  tonic::DartPersistentValue view_connected_callback_;
  tonic::DartPersistentValue view_disconnected_callback_;
  tonic::DartPersistentValue view_state_changed_callback_;
  std::string isolate_service_id_;
  scenic::ResourceId resource_id_ = 0;
  zx_koid_t koid_ = ZX_KOID_INVALID;

  fml::WeakPtrFactory<SceneHost> weak_factory_;

  FML_DISALLOW_COPY_AND_ASSIGN(SceneHost);
};

}  // namespace flutter

#endif  // FLUTTER_LIB_UI_COMPOSITING_SCENE_HOST_H_
