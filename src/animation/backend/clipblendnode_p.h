/****************************************************************************
**
** Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QT3DANIMATION_ANIMATION_CLIPBLENDNODE_P_H
#define QT3DANIMATION_ANIMATION_CLIPBLENDNODE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of other Qt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <Qt3DCore/qnodeid.h>
#include <Qt3DAnimation/private/backendnode_p.h>
#include <Qt3DAnimation/private/managers_p.h>

QT_BEGIN_NAMESPACE

namespace Qt3DAnimation {

namespace Animation {

class ClipBlendNodeManager;

class Q_AUTOTEST_EXPORT ClipBlendNode : public BackendNode
{
public:
    ~ClipBlendNode();

    enum BlendType {
        NoneBlendType,
        LerpBlendType
    };

    void setClipBlendNodeManager(ClipBlendNodeManager *manager);

    ClipBlendNodeManager *clipBlendNodeManager() const;
    BlendType blendType() const;
    Qt3DCore::QNodeId parentId() const;
    Qt3DCore::QNodeIdVector childrenIds() const;
    Qt3DCore::QNodeIdVector clipIds() const;

    void sceneChangeEvent(const Qt3DCore::QSceneChangePtr &e) Q_DECL_OVERRIDE;

protected:
    explicit ClipBlendNode(BlendType blendType);
    void initializeFromPeer(const Qt3DCore::QNodeCreatedChangeBasePtr &change) Q_DECL_OVERRIDE;

private:
    void setParentId(Qt3DCore::QNodeId parentId);
    void addChildId(Qt3DCore::QNodeId childId);
    void removeChildId(Qt3DCore::QNodeId childId);

    // Can either contain clips or nothing (tree of other blend nodes)
    Qt3DCore::QNodeIdVector m_clipIds;

    Qt3DCore::QNodeId m_parentId;
    Qt3DCore::QNodeIdVector m_childrenIds;
    ClipBlendNodeManager *m_manager;
    BlendType m_blendType;
};

template<typename Backend, typename Frontend>
class ClipBlendNodeFunctor : public Qt3DCore::QBackendNodeMapper
{
public:
    explicit ClipBlendNodeFunctor(Handler *handler, ClipBlendNodeManager *manager)
        : m_handler(handler)
        , m_manager(manager)
    {
    }

    Qt3DCore::QBackendNode *create(const Qt3DCore::QNodeCreatedChangeBasePtr &change) const Q_DECL_FINAL
    {
        if (m_manager->containsNode(change->subjectId()))
            return static_cast<Backend *>(m_manager->lookupNode(change->subjectId()));
        Backend *backend = new Backend();
        backend->setClipBlendNodeManager(m_manager);
        backend->setHandler(m_handler);
        m_manager->appendNode(change->subjectId(), backend);
        return backend;
    }

    Qt3DCore::QBackendNode *get(Qt3DCore::QNodeId id) const Q_DECL_FINAL
    {
        return m_manager->lookupNode(id);
    }

    void destroy(Qt3DCore::QNodeId id) const Q_DECL_FINAL
    {
        m_manager->releaseNode(id);
    }

private:
    Handler *m_handler;
    ClipBlendNodeManager *m_manager;
};

} // Animation

} // Qt3DAnimation

QT_END_NAMESPACE

#endif // QT3DANIMATION_ANIMATION_CLIPBLENDNODE_P_H