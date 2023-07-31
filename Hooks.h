#pragma once

#define Thiz_func(t,m) t = (decltype(t)) m



template<typename T>
struct IL2CppArray : Il2CppObject  {
    Il2CppArrayBounds *bounds;
    il2cpp_array_size_t capacity;
    T m_Items[0];

    il2cpp_array_size_t getCapacity() {
        if (!this)
            return 0;
        return capacity;
    }

    T *getPointer() {
        if (!this)
            return nullptr;
        return m_Items;
    }

    std::vector<T> toCPPlist() {
        if (!this)
            return {};

        std::vector<T> ret;

        for (int i = 0; i < capacity; i++)
            ret.push_back(m_Items[i]);
        return std::move(ret);
    }

    bool copyFrom(const std::vector<T> &vec) {
        if (!this)
            return false;
        return copyFrom((T *)vec.data(), (int)vec.size());
    }

    bool copyFrom(T *arr, int size) {

        if (!this)
            return false;
        if (size < capacity)
            return false;
        memcpy(m_Items, arr, size  *sizeof(T));
        return true;
    }

    void copyTo(T *arr) {
        if (!this || !CheckObj(m_Items))
            return;
        memcpy(arr, m_Items, sizeof(T) *capacity);
    }

    T operator[] (int index) {
        if (getCapacity() < index)
            return {};
        return m_Items[index];
    }

    T at(int index) {
        if (!this || getCapacity() <= index || empty())
            return {};
        return m_Items[index];
    }

    bool empty() {
        if (!this)
            return false;
        return getCapacity() <= 0;
    }

    static IL2CppArray<T> *Create(int capacity) {
        auto monoArr = (IL2CppArray<T> *)malloc(sizeof(IL2CppArray) + sizeof(T)  *capacity);
        monoArr->capacity = capacity;
        return monoArr;
    }

    static IL2CppArray<T> *Create(const std::vector<T> &vec) {
        return Create(vec.data(), vec.size());
    }

    static IL2CppArray<T> *Create(T *arr, int size) {
        IL2CppArray<T> *monoArr = Create(size);
        monoArr->copyFrom(arr, size);
        return monoArr;
    }
};

template<typename T>
struct IL2CppList : Il2CppObject {
    IL2CppArray<T> *items;
    int size;
    int version;

    T *getItems() {
        return items->getPointer();
    }

    int getSize() {
        return size;
    }

    int getVersion() {
        return version;
    }

    std::vector<T> toCPPlist() {
        std::vector<T> ret;

        for (int i = 0; i < size; i++)
            ret.push_back(getItems()[i]);
        return std::move(ret);
    }

    void Add(T val) {
        GrowIfNeeded(1);
        items->m_Items[size] = val;
        size++;
        version++;
    }

    int IndexOf(T val) {
        for (int i = 0; i < size; i++)
            if (items->m_Items[i] == val)
                return i;
        return -1;
    }

    void RemoveAt(int index) {
        if (index != -1) {
            Shift(index, -1);
            version++;
        }
    }

    void Remove(T val) {
        RemoveAt(IndexOf(val));
    }

    bool Resize(int newCapacity) {
        if (!this)
            return false;

        if (newCapacity <= items->capacity)
            return false;

        auto nItems = IL2CppArray<T>::Create(newCapacity);
        nItems->klass = items->klass;
        nItems->monitor = items->monitor;
        nItems->bounds = items->bounds;
        nItems->capacity = newCapacity;
        if (items->capacity > 0)
            memcpy(nItems->m_Items, items->m_Items, items->capacity *sizeof(T));
        items = nItems;
        return true;
    }

    T operator[] (int index) {
        return items->m_Items[index];
    }

private:
    void GrowIfNeeded(int n) {
        if (size + n > items->capacity)
            Resize(size + n);
    }

    void Shift(int start, int delta) {
        if (delta < 0)
            start -= delta;
        if (start < size)
            memcpy(items + start + delta, items + start, size - start);
        size += delta;
        if (delta < 0)
            memset(items + size + delta, 0, -delta  *sizeof(T));
    }
};

template<typename TKey, typename TValue>
struct IL2CppDictionary {

    struct Entry {
        int hashCode, next;
        TKey key;
        TValue value;
    };

    Il2CppClass *klass;
    MonitorData *monitor;
    IL2CppArray<int> *buckets;
    IL2CppArray<Entry> *entries;
    int count;
    int version;
    int freeList;
    int freeCount;
    void *comparer;
    IL2CppArray<TKey> *keys;
    IL2CppArray<TValue> *values;
    void *syncRoot;

    std::map<TKey, TValue> toMap() {
        std::map<TKey, TValue> ret;
        for (auto it = (Entry *)&entries->m_Items; it != ((Entry *)&entries->m_Items + count); ++it) ret.emplace(std::make_pair(it->key, it->value));
        return std::move(ret);
    }

    std::vector<TKey> getKeys() {
        std::vector<TKey> ret;
        for (int i = 0; i < count; ++i) ret.emplace_back(entries->at(i).key);
        return std::move(ret);
    }

    std::vector<TValue> getValues() {
        std::vector<TValue> ret;
        for (int i = 0; i < count; ++i) ret.emplace_back(entries->at(i).value);
        return std::move(ret);
    }

    int getSize() {
        return count;
    }

    int getVersion() {
        return version;
    }

    TValue Get(TKey key) {
        TValue ret;
        if (TryGet(key, &ret)) return ret;
        return {};
    }

    TValue operator[](TKey key) {
        return Get(key);
    }
};

void DroneView() {
    if (GetFov == 0) {
        GetFov = Camera::get_fieldOfView();
    }
    if (SetFov > 0 && GetFov != 0) {
        Camera::set_fieldOfView((float)GetFov + SetFov);
    }
    if (SetFov == 0 && GetFov != 0) {
        Camera::set_fieldOfView((float)GetFov);
    }

}



void (*oISGameMain_Update)(void *instance);
void ISGameMain_Update(void *instance) {
    if(instance != NULL) {
        void *LogicBattleManager_Instance = BattleLogicManager->GetFieldByName<void*>("Instance").get();
        if(LogicBattleManager_Instance != NULL)
        response.BattleState = *(int *) ((uint64_t) LogicBattleManager_Instance + Fields["LogicBattleManager::_m_eState"]); // private eBattleState _m_eState; // 0xA4

    }

    return oISGameMain_Update(instance);
}

#define SetHook(RET, NAME, ARGS) \
    RET (*o ## NAME) ARGS; \
    RET NAME ARGS

int Thiz_m_self_camp = -1;

void (*oBattleBridge_SetMapInvisibility)(void * ,uint32_t,bool,bool);
void BattleBridge_SetMapInvisibility(void *thiz,uint32_t uGuid, bool bflag, bool bDeath) {
    if(thiz != nullptr && Thiz_AaModz && (Show.Player.Minimap || Show.Monster.Minimap)){
        void *BattleManager_Instance = BattleMgr->GetFieldByName<void*>("Instance").get();
        if(BattleManager_Instance != nullptr){
            void *entity = BattleManager_GetEntityBase(BattleManager_Instance,uGuid);
            if(entity != nullptr){
                bool isPlayer = EntityBase_isPlayer(entity);
                if(isPlayer && Show.Player.Minimap){
                    int campType = GetCampType(BattleManager_Instance, entity, false);
                    void *self_player = GetPlayerSelf(BattleManager_Instance);
                    if(self_player){
                        Thiz_m_self_camp = GetCampType(BattleManager_Instance, self_player, false);
                        if(campType != Thiz_m_self_camp){
                            return oBattleBridge_SetMapInvisibility(thiz,uGuid,true,bDeath);
                        }
                    }
                }
                bool isMonster = EntityBase_isMonster(entity);
                if(isMonster && Show.Monster.Minimap){
                    int campType = GetCampType(BattleManager_Instance,entity, false);
                    void *self_player = GetPlayerSelf(BattleManager_Instance);
                    if(self_player){
                        Thiz_m_self_camp = GetCampType(BattleManager_Instance, self_player, false);
                        if(campType != Thiz_m_self_camp){
                            return oBattleBridge_SetMapInvisibility(thiz,uGuid,true,bDeath);
                        }
                    }
                }
            }
        }
    }
    return oBattleBridge_SetMapInvisibility(thiz,uGuid,bflag,bDeath);
}

void (*oBattleBridge_SetBloodInvisibility)(void*,uint32_t uGuid, bool bflag, bool bDeath, bool isForce, int Location);
void BattleBridge_SetBloodInvisibility(void* thiz,uint32_t uGuid, bool bflag, bool bDeath, bool isForce, int Location) {
    if(thiz != nullptr && (Show.Player.HpBar || Show.Player.HpBarIcon)){
        void *BattleManager_Instance = BattleMgr->GetFieldByName<void*>("Instance").get();
        if(BattleManager_Instance != nullptr){
            void *entity = BattleManager_GetEntityBase(BattleManager_Instance,uGuid);
            if(entity != nullptr){
                bool isPlayer = *(bool *)((uint64_t) entity + Fields["EntityBase::IsPlayer"]);

                if(isPlayer){
                    int campType = GetCampType(BattleManager_Instance, entity, false);
                    void *self_player = GetPlayerSelf(BattleManager_Instance);
                    if(self_player){
                        Thiz_m_self_camp = GetCampType(BattleManager_Instance, self_player, false);
                        if(campType != Thiz_m_self_camp){
                            return oBattleBridge_SetBloodInvisibility(thiz,uGuid,true,bDeath,true,0);
                        }
                    }
                }
            }
        }
    }
    return oBattleBridge_SetBloodInvisibility(thiz,uGuid,bflag,bDeath,isForce,Location);
}


void (*oUIHeadInfo_LateUpdate)(void *thiz);
void UIHeadInfo_LateUpdate(void *thiz){

    if(Show.Monster.Minimap || Show.Monster.All){
    void *m_BattleBridge = BattleDatas->GetFieldByName<void*>("m_BattleBridge").get();
    if(m_BattleBridge != nullptr){
        void *BattleManager_Instance = BattleMgr->GetFieldByName<void*>("Instance").get();
        if(BattleManager_Instance != nullptr){
            auto MonsterList = *(monoList<void **> **)((dword)BattleManager_Instance + Fields["BattleManager::m_ShowMonsters"]);
            if(MonsterList){
                for(int i = 0;i < MonsterList->getSize();i++){
                    void *entity = MonsterList->getItems()[i];
                    if(entity == nullptr) continue;
                    int campType = GetCampType(BattleManager_Instance, entity, false);
                    void *self_player = GetPlayerSelf(BattleManager_Instance);
                    if(self_player == nullptr) continue;
                    int selfCamp = GetCampType(BattleManager_Instance, self_player, false);
                    bool isDeath = EntityBase_m_bDeath(entity);
                    auto m_IsDeathProgress = EntityBase_m_IsDeathProgress(entity);
                    if(isDeath || m_IsDeathProgress) continue;

                    uint32_t uGuid = EntityBase_m_uGuid(entity);
                    bool bShowEntityLayer = ShowEntity_bShowEntityLayer(entity);

                    if(Show.Monster.Minimap && !bShowEntityLayer ){
                        Vector3 pos = ShowEntity_get_Position(entity);
                        BattleBridge_SetMapEntityIconPos(m_BattleBridge,pos,uGuid,true);
                        oBattleBridge_SetMapInvisibility(m_BattleBridge,uGuid,true,false);
                    }
                    if(Show.Monster.All && !bShowEntityLayer ){
                        Vector3 pos = ShowEntity_get_Position(entity);
                        BattleBridge_SetBloodInvisibility(m_BattleBridge,uGuid,true,isDeath,false,0);
                        BattleBridge_UpdateBloodPos(m_BattleBridge,entity, true);
                        oBattleBridge_SetMapInvisibility(m_BattleBridge,uGuid,true,isDeath);
                        ShowEntity_UpdateEyeLayer(entity, true, false,false);
                    }
                }
            }
        }
    }
 }

    if(thiz != nullptr){
        void *m_BattleBridge = BattleDatas->GetFieldByName<void*>("m_BattleBridge").get();
        if(m_BattleBridge != nullptr){
            void *BattleManager_Instance = BattleMgr->GetFieldByName<void*>("Instance").get();
            if(BattleManager_Instance != nullptr){
                auto playerList = *(monoList<void **> **)((dword)BattleManager_Instance + Fields["BattleManager::m_ShowPlayers"]);
                if(playerList){
                    for(int i = 0;i < playerList->getSize();i++){
                        void *entity = playerList->getItems()[i];
                        if(entity == nullptr) continue;
                        int campType = GetCampType(BattleManager_Instance, entity, false);
                        void *self_player = GetPlayerSelf(BattleManager_Instance);
                        if(self_player == nullptr) continue;
                        int selfCamp = GetCampType(BattleManager_Instance, self_player, false);
                        bool isDeath = EntityBase_m_bDeath(entity);
                        if(isDeath) continue;

                       
                        uint32_t uGuid = EntityBase_m_uGuid(entity);
                        bool bShowEntityLayer = ShowEntity_bShowEntityLayer(entity);

                        if(Show.Player.Minimap && !bShowEntityLayer ){
                            Vector3 pos = ShowEntity_get_Position(entity);
                            BattleBridge_SetMapEntityIconPos(m_BattleBridge,pos,uGuid,true);
                            oBattleBridge_SetMapInvisibility(m_BattleBridge,uGuid,true,false);
                        }
                          if(Show.Player.HpBar && !bShowEntityLayer) {
                            BattleBridge_UpdateBloodPos(m_BattleBridge, entity, true);
                            BattleBridge_SynBloodAttr(m_BattleBridge, entity, true);
                        }else{
                            BattleBridge_UpdateBloodPos(m_BattleBridge,entity,true);
                            BattleBridge_SynBloodAttr(m_BattleBridge,entity,true);
                        }
                    }
                }
            }
        }
    }
    oUIHeadInfo_LateUpdate(thiz);
}


void DrawESPCoolDown(ImDrawList *draw) {
    if (response.BattleState == 6) {
        void *Instance = BattleLogicManager->GetFieldByName<void*>("Instance").get();
        if (Instance != nullptr) {
            auto m_dicPlayerLogic = *(IL2CppDictionary<uint32_t, void *> **)((uintptr_t)Instance + Fields["LogicBattleManager::m_dicPlayerLogic"]);
            if (m_dicPlayerLogic != nullptr) {

                for (int i = 0; i < m_dicPlayerLogic->getSize(); i++) {

                    void *logicFighter = m_dicPlayerLogic->getValues()[i];
                    if (logicFighter != nullptr) {

                        bool m_bDeath = EntityBase_m_bDeath(logicFighter);
                        bool m_bSameCampType = EntityBase_m_bSameCampType(logicFighter);
                        int m_ID = EntityBase_m_ID(logicFighter);
                        Vector3 root_Pos = Player_get_position(logicFighter);
                        auto RootPosW2S = WorldToScreen(root_Pos);
                        auto RootPosVec2 = Vector2(glWidth- RootPosW2S.x, RootPosW2S.y);
                        if (RootPosW2S.z > 0) {
                            RootPosVec2 = Vector2(RootPosW2S.x, glHeight - RootPosW2S.y);
                        }

                        auto From = ImVec2(glWidth /2, (glHeight / 2) + 30);
                        ImVec2 location(RootPosVec2.X, (glHeight - RootPosVec2.Y));

                        if (!m_bDeath && !m_bSameCampType) {
                            uint32_t CD[8] {0};

                            uintptr_t m_SkillComp = LogicFighter_m_SkillComp(logicFighter);
                            if (m_SkillComp != NULL) {
                                uintptr_t m_CoolDownComp = *(uintptr_t *)((uintptr_t)m_SkillComp + Fields["LogicSkillComp::m_CoolDownComp"]);
                                if (m_CoolDownComp != NULL) {
                                    IL2CppDictionary<int, uintptr_t> *m_DicCoolInfo = *(IL2CppDictionary<int, uintptr_t> **)((uintptr_t)m_CoolDownComp + Fields["CoolDownComp::m_DicCoolInfo"]);
                                    if (m_DicCoolInfo != NULL) {

                                        for (int u = 0; u < m_DicCoolInfo->getSize(); u++) {
                                            auto values = m_DicCoolInfo->getValues()[u];
                                            auto keys = m_DicCoolInfo->getKeys()[u];

                                            if (values && keys) {
                                                uint32_t uiCoolTime = ((uint32_t (*)(uintptr_t))(Methods["CoolDownData::GetCoolTime"]))(values);
                                                int skillIdx = -1;

                                                if (SpellIdExist(keys)) {
                                                    skillIdx = 4;
                                                } else if (keys % 100 == 10) {
                                                    skillIdx = 0;
                                                } else if (keys % 100 == 20) {
                                                    skillIdx = 1;
                                                } else if (keys % 100 == 30) {
                                                    skillIdx = 2;
                                                } else if (keys == 100 * m_ID + 10 || keys == 2010510) {
                                                    skillIdx = 0;
                                                } else if (keys == 100 * m_ID + 20 || keys == 2010520) {
                                                    skillIdx = 1;
                                                } else if (keys == 100 * m_ID + 30 || keys == 2010530) {
                                                    skillIdx = 2;
                                                } else if (keys == 100 * m_ID + 40) {
                                                    skillIdx = 3;
                                                }

                                                if (skillIdx != -1) {
                                                    if (uiCoolTime > 0) {
                                                        CD[skillIdx] = uiCoolTime / 1000;
                                                    } else {
                                                        CD[skillIdx] = 0;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            if(Esp.Player.Cooldown){
                                std::string strCooldown = "";
                                for (int i = 0; i < 3; i++) {
                                    if (!strCooldown.empty())
                                        strCooldown += " - ";
                                    if (CD[i]) {
                                        strCooldown += " ";
                                        strCooldown += std::to_string(CD[i]);
                                        strCooldown += " s ";
                                    } else

                                        strCooldown += "  \xef\x84\x91  ";
                                }

                                ImVec2 textAlign = ImGui::CalcTextSize2(strCooldown.c_str(), 0, ((float) glHeight / 39.0f));
                                draw->AddText(NULL, ((float) glHeight / 39.0f), {RootPosVec2.X - (textAlign.x / 2), RootPosVec2.Y + CDPOS + ubahpos}, IM_COL32(255, 255, 255, 255), strCooldown.c_str());

                            }

                        }
                    }
                }
            }
        }
    }
}
 void DrawESP(ImDrawList *draw,int screenWidth,int screenHeight) {
    void *BattleBridge_Instance = BattleDatas->GetFieldByName<void *>("m_BattleBridge").get();
    if (BattleBridge_Instance){
        ImVec2 screen(glWidth, glHeight);
        Draw_info_Room(draw);
        DrawESPCoolDown(draw);
        auto _bStartBattle = BattleBridge_bStartBattle(BattleBridge_Instance);
        if (_bStartBattle) {
         DroneView();
          void *BattleManager_Instance = BattleMgr->GetFieldByName<void*>("Instance").get();
            if (BattleManager_Instance) {
                float mScale = glHeight / (float) 1002;
                auto m_LocalPlayerShow =  Battlemanager_m_LocalPlayerShow(BattleManager_Instance);
                if (m_LocalPlayerShow) {
                    auto selfPos = *(Vector3 *) ((uintptr_t)m_LocalPlayerShow + Fields["ShowEntity::_Position"]);
                    if (selfPos != Vector3::zero()) {
                        auto SelfPosW2S = WorldToScreen(selfPos);
                        auto SelfPosVec2 = ImVec2(glWidth - SelfPosW2S.x, SelfPosW2S.y);
                        if (SelfPosW2S.z > 0) {
                            SelfPosVec2 = ImVec2(SelfPosW2S.x, glHeight - SelfPosW2S.y);
                        }
                        monoList<void **> *m_ShowPlayers = *(monoList<void **> **) ((uintptr_t)BattleManager_Instance + Fields["BattleManager::m_ShowPlayers"]);
                        if (m_ShowPlayers) {
                            for (int i = 0; i < m_ShowPlayers->getSize(); i++) {
                                auto Pawn = m_ShowPlayers->getItems()[i];
                                if (!Pawn) continue;
                                auto m_bSameCampType = EntityBase_m_bSameCampType(Pawn);
                                if (m_bSameCampType) continue;
                                auto m_bDeath = EntityBase_m_bDeath(Pawn);
                                if (m_bDeath) continue;
                                auto bShowEntityLayer = ShowEntity_bShowEntityLayer(Pawn);
                                auto CurHP = EntityBase_m_Hp(Pawn);
                                auto MaxHP = EntityBase_m_HpMax(Pawn);
                                auto _Position = ShowEntity_Position(Pawn);
                                auto Mp = EntityBase_Mp(Pawn);
                                auto MpMax = EntityBase_MpMax(Pawn);
                                auto m_Xp = EntityBase_m_Xp(Pawn);
                                auto m_XpMax = EntityBase_m_XpMax(Pawn);
                                auto *m_RoleName = ShowEntity_m_RoleName(Pawn);
                                auto *m_HeroName = ShowPlayer_m_HeroName(Pawn);
                                auto Herolevel = EntityBase_m_Levels(Pawn);
                                auto HeroID = EntityBase_m_ID(Pawn);

                                int Distance = (int) Vector3::Distance(selfPos, _Position);
                                auto RootPosW2S = WorldToScreen(_Position);
                                auto RootPosVec2 = Vector2(glWidth- RootPosW2S.x, RootPosW2S.y);
                                if (RootPosW2S.z > 0) {
                                    RootPosVec2 = Vector2(RootPosW2S.x, glHeight - RootPosW2S.y);
                                }

                                auto LeftHandPosVec2 = Vector2(RootPosVec2.X - (glWidth / 10.35), RootPosVec2.Y);
                                auto RightHandPosVec2 = Vector2(RootPosVec2.X + (glHeight / 10.35), RootPosVec2.Y);
                                auto HeadPosVec2 = Vector2(RootPosVec2.X, RootPosVec2.Y - (glHeight / 10.35));
                                auto FootPosVec2 = Vector2(RootPosVec2.X, RootPosVec2.Y + (glHeight / 10.35));

                                auto From = ImVec2(glWidth /2, (glHeight / 2) + 30);
                                ImVec2 location(RootPosVec2.X, (glHeight - RootPosVec2.Y));
                                int borders = isOutsideSafezone(location, screen);
                                ImVec2 TextRenderPos = pushScreenBorder(location, screen, borders, -(int)((mScale * 65)));
                                #define To RootPosVec2

                               ImVec2 lockPost = ImVec2(To.X, To.Y);
                                if(bShowEntityLayer && Distance < 6.0f)continue;

                                if (Esp.Player.Visible) {
                                    if (bShowEntityLayer)continue;
                                }

                                if (Esp.Player.Line || Esp.Player.All) {
                                    Alpha = 200;
                                    ImColor Clr4 = IColor::Black();

                                    if (Distance < 100)
                                        Clr4 = IColor::Green();
                                    if (Distance < 70)
                                        Clr4 = IColor::DogerBlue();
                                    if (Distance < 50)
                                        Clr4 = IColor::Cyan();
                                    if (Distance < 30)
                                        Clr4 = IColor::Yellow();
                                    if (Distance < 20)
                                        Clr4 = IColor::Purple();

                                    draw->AddCircle( lockPost, 5, ImColor(255,255,255,140));
                                    draw->AddCircleFilled(lockPost, 4, Clr4);
                                    draw->AddCircle( From, 5, ImColor(255,255,255,140));
                                    draw->AddCircleFilled( From, 4, Clr4);
                                }
                                if (Esp.Player.Line || Esp.Player.All) {
                                    ImU32 clr = IM_COL32(0, 255, 255, 80);
                                    ImU32 clr2 = COL (RLine, GLine, BLine, Alpha);

                                    if (CurHP  < (MaxHP * 0.3)) {
                                        RLine = 255;
                                        clr = COL(RLine, 0, 0, 200);
                                        clr2 = COL(RLine, 0, 0, 120);
                                    }
                                    draw->AddLine(ImVec2(From.x,From.y + 1), ImVec2(To.X, To.Y + 1), clr , SizeLine);
                                    draw->AddLine(From, ImVec2(To.X, To.Y), clr2 , SizeLine3);
                                    draw->AddLine(ImVec2(From.x,From.y -1), ImVec2(To.X, To.Y -1), clr , SizeLine);

                                }

                                if (Esp.Player.Box || Esp.Player.All) {
                                    ImVec2 v1 = ImVec2(To.X - (Box / 2), To.Y);
                                    ImVec2 v2 = ImVec2(To.X + (Box / 2), To.Y);
                                    ImVec2 v3 = ImVec2(To.X + (Box / 2), To.Y - Box);
                                    ImVec2 v4 = ImVec2(To.X - (Box / 2), To.Y - Box);
                                    ImU32 clr = COL (RLine, GLine, BLine, Alpha);
                                    if (CurHP  < (MaxHP * 0.3)) {
                                        clr = COL(RLine, 0, 0, Alpha);
                                    }
                                    draw->AddLine( v1, v2,clr, SizeLine);
                                    draw->AddLine( v2, v3,clr, SizeLine);
                                    draw->AddLine( v3, v4,clr, SizeLine);
                                    draw->AddLine( v4, v1,clr, SizeLine);
                                }

                                if (Esp.Player.esp_infobar || Esp.Player.All) {
                                    int MP = Mp;
                                    int MaXMP = MpMax;
                                    int Xp = m_Xp;
                                    int XpMax = m_XpMax;
                                    int m_HeroID = HeroID;


                                    if(bShowEntityLayer)continue; {
                                        draw_hp(ImVec2(To.X - (HP_120 / 2),To.Y + 54 + BarPos + ubahpos),CurHP,MaxHP,HP_120);
                                        draw_manabase(ImVec2(To.X - (HP_120 / 2),To.Y + 60 + BarPos + ubahpos),0,0,HP_120);
                                        draw_mp(ImVec2(To.X - (HP_120 / 2),To.Y + 60 + BarPos + ubahpos),MP,MaXMP,HP_120);

                                        if (HeroIDToType(m_HeroID) == "RedGroup"){
                                            draw_Red(ImVec2(To.X - (HP_120 / 2), To.Y -99 + ubahpos),Xp, XpMax, HP_120);
                                        }else if (HeroIDToType(m_HeroID) == "White"){
                                            draw_White(ImVec2(To.X - (HP_120 / 2), To.Y -99 + ubahpos),Xp, XpMax, HP_120);
                                        }else if (HeroIDToType(m_HeroID) == "RedWhith"){
                                            draw_Xp2(ImVec2(To.X - (HP_120 / 2), To.Y -99 + ubahpos),Xp, XpMax, HP_120);
                                        }else if (HeroIDToType(m_HeroID) == "HeroXp"){
                                            draw_XpTrans(ImVec2(To.X - (HP_120 / 2), To.Y -99 + ubahpos),Xp, XpMax, HP_120);
                                        }else{
                                            draw_Xp(ImVec2(To.X - (HP_120 / 2), To.Y -99 + ubahpos),Xp, XpMax, HP_120);
                                        }

                                       draw->AddCircleFilled(ImVec2(To.X + LevelPos, To.Y + 55 + BarPos + ubahpos), (mScale * 25), COL(0, 0, 0, 255));
                                       draw->AddCircleFilled(ImVec2(To.X + LevelPos, To.Y + 55 + BarPos + ubahpos), (mScale * 18), COL(54, 54, 54, 240));

                                        int m_Level = Herolevel;

                                        if(m_Level > 9){
                                            draw->AddText(NULL,20.0f,ImVec2(To.X + -75, To.Y + LevelPosY + ubahpos),COL(255, 255, 255,Alpha),std::to_string(m_Level).c_str());
                                        }else{
                                            draw->AddText(NULL,20.0f,ImVec2(To.X + -72, To.Y + LevelPosY + ubahpos),COL(255, 255, 255,Alpha),std::to_string(m_Level).c_str());
                                        }}
                                }
                                if (Esp.Player.EnemyName || Esp.Player.HeroName || Esp.Player.All) {
                                    if(bShowEntityLayer)continue; {
                                    if (Esp.Player.EnemyName && Esp.Player.HeroName) {

                                        if (m_RoleName && m_HeroName) {
                                            std::string strName = "[" + m_HeroName->toString() + "] " + m_RoleName->toString();
                                            auto textSize = ImGui::CalcTextSize2(strName.c_str(), 0, ((float) glHeight / 39.0f));
                                            ImU32 TxtColor = COL(0, 0, 0, 255);
                                            draw->AddText(NULL, ((float) glHeight / 39.0f), {RootPosVec2.X - (textSize.x / 2), RootPosVec2.Y + 25}, IM_COL32(RText, GText, BText, Alpha), strName.c_str());
                                            draw->AddText(NULL, ((float) glHeight / 39.0f), {RootPosVec2.X - (textSize.x / 2), RootPosVec2.Y + 25}, IM_COL32(RText, GText, BText, Alpha), strName.c_str());

                                        }

                                    } else if (Esp.Player.EnemyName) {
                                        if (m_RoleName) {
                                            std::string strName = m_RoleName->toString();
                                            auto textSize = ImGui::CalcTextSize2(strName.c_str(), 0, ((float) glHeight / 39.0f));
                                            draw->AddText(NULL, ((float) glHeight / 39.0f), {RootPosVec2.X - (textSize.x / 2), RootPosVec2.Y + 25}, IM_COL32(RText, GText, BText, Alpha), strName.c_str());
                                            draw->AddText(NULL, ((float) glHeight / 39.0f), {RootPosVec2.X - (textSize.x / 2), RootPosVec2.Y + 25}, IM_COL32(RText, GText, BText, Alpha), strName.c_str());
                                        }
                                    } else if (Esp.Player.HeroName) {
                                        if (m_HeroName) {
                                            std::string strName = "[" + m_HeroName->toString() + "]";
                                            auto textSize = ImGui::CalcTextSize2(strName.c_str(), 0, ((float) glHeight / 39.0f));
                                            draw->AddText(NULL, ((float) glHeight / 39.0f), {RootPosVec2.X - (textSize.x / 2), RootPosVec2.Y + 25}, IM_COL32(RText, GText, BText, Alpha), strName.c_str());
                                            draw->AddText(NULL, ((float) glHeight / 39.0f), {RootPosVec2.X - (textSize.x / 2), RootPosVec2.Y + 25}, IM_COL32(RText, GText, BText, Alpha), strName.c_str());
                                        } }
                                    }
                                }

                                    if (Esp.Player.All && isOutsideScreen(ImVec2(To.X, To.Y), ImVec2(glWidth, glHeight)) || Esp.Player.Alert && isOutsideScreen(ImVec2(To.X, To.Y), ImVec2(glWidth, glHeight))) {
                                        ImVec2 DotRenderPos = pushToScreen(ImVec2(To.X, To.Y ), ImVec2(glWidth,glHeight), - 50); // Border Position
                                        ImVec2 hintTextRenderPos = pushToScreen(ImVec2(To.X, To.Y ), ImVec2(glWidth, glHeight), - 50);  // Distance


                                    draw->AddCircleFilled(DotRenderPos, (mScale * 60), IM_COL32(0, 0, 0, 130));
                                    draw->AddCircle(DotRenderPos, (mScale * 60),ImColor(RAlert,GAlert,BAlert,180), 0, 5); // CIRCLE HEALTH
                                    ImU32 clr = COL (RAlert, GAlert, BAlert, 155);
                                    if (CurHP < (MaxHP * 0.3)) {

                                        clr = COL(255, 0, 0, 180);
                                    }
                                    draw->AddCircle(DotRenderPos, (mScale * 60),clr, 0, 4); // CIRCLE HEALTH
                                    std::string strDistance = to_string(Distance) + " M";
                                    auto textSize = ImGui::CalcTextSize2(strDistance.c_str(), 0, ((float) glHeight / 45.0f));
                                    draw->AddText(NULL, ((float) glHeight / 39.0f), {hintTextRenderPos.x - (textSize.x / 2), hintTextRenderPos.y - distanceAlert}, IM_COL32(255, 255, 255, 255), strDistance.c_str());
                                    if (m_HeroName) {
                                        std::string strName = m_HeroName->toString();
                                        auto textSize = ImGui::CalcTextSize2(strName.c_str(), 0, ((float) glHeight / 39.0f));
                                        draw->AddText(NULL, ((float) glHeight / 39.0f), {hintTextRenderPos.x - (textSize.x / 2), hintTextRenderPos.y - 7}, IM_COL32(255, 255, 255, 255), strName.c_str());
                                    }
                                    if (m_HeroName) {
                                        draw_x({hintTextRenderPos.x - (textSize.x / 2) + HealthX, hintTextRenderPos.y + HealthAlert},CurHP, MaxHP,60);
                                    }
                                }
                                if (Esp.Player.Rounded) {
                                    Esp.Player.esp_infobar = false;
                                    Esp.Player.Box = false;
                                    Esp.Player.HeroName = false;
                                    Esp.Player.EnemyName = false;
                                    if(bShowEntityLayer)continue;
                                    ImVec2 DotRenderPos2 = ImVec2(To.X, To.Y - 43); // Border Position
                                    ImVec2 hintTextRenderPos2 = ImVec2(To.X, To.Y - 43);  // Distance
                                    draw->AddCircleFilled(DotRenderPos2, (mScale * RoundSize), IM_COL32(0, 0, 0, 130));
                                    draw->AddCircle(DotRenderPos2, (mScale * RoundSize),ImColor(RAlert,GAlert,BAlert,180), 0, 6); // CIRCLE HEALTH
                                    ImU32 clr = COL (RAlert, GAlert, BAlert, 155);
                                    if (CurHP < (MaxHP * 0.3)) {
                                        clr = COL(255, 0, 0, 180);
                                    }
                                    draw->AddCircle(DotRenderPos2, (mScale * RoundSize),clr, 0, 4); // CIRCLE HEALTH
                                    std::string strDistance = "Level " + to_string(Herolevel);
                                    auto textSize = ImGui::CalcTextSize2(strDistance.c_str(), 0, ((float) glHeight / 45.0f));
                                    draw->AddText(NULL, ((float) glHeight / 39.0f), {hintTextRenderPos2.x - (textSize.x / 2), hintTextRenderPos2.y - distanceAlert}, IM_COL32(255, 255, 255, 255), strDistance.c_str());
                                    if (Esp.Player.Rounded) {
                                        std::string strName = m_HeroName->toString();
                                        auto textSize = ImGui::CalcTextSize2(strName.c_str(), 0, ((float) glHeight / 39.0f));
                                        draw->AddText(NULL, ((float) glHeight / 39.0f), {hintTextRenderPos2.x - (textSize.x / 2), hintTextRenderPos2.y - 7}, IM_COL32(255, 255, 255, 255), strName.c_str());
                                    }
                                    if (Esp.Player.Rounded) {
                                        draw_x({hintTextRenderPos2.x - (textSize.x / 2) + HealthX, hintTextRenderPos2.y + HealthAlert},CurHP, MaxHP,RoundSize);
                                    }
                                }
                                }

                            }
                        }




                    monoList<void **> *m_ShowMonsters = *(monoList<void **> **) ((uintptr_t)BattleManager_Instance + Fields["BattleManager::m_ShowMonsters"]);
                    if (m_ShowMonsters) {
                        for (int i = 0; i < m_ShowMonsters->getSize(); i++) {
                            auto isMonster = m_ShowMonsters->getItems()[i];
                            if (!isMonster) continue;
                            auto m_ID =  EntityBase_m_ID(isMonster);
                            auto MonsterType = EntityBase_m_iType(isMonster);
                            if (MonsterToType(MonsterType) == "") continue;
                            auto m_bSameCampType = EntityBase_m_bSameCampType(isMonster);
                            if (m_bSameCampType) continue;
                            auto m_bDeath = EntityBase_m_bDeath(isMonster);
                            if (m_bDeath) continue;
                            auto m_IsDeathProgress = EntityBase_m_IsDeathProgress(isMonster);
                            if (m_IsDeathProgress) continue;
                            auto CurHP = EntityBase_m_Hp(isMonster);
                            auto MaxHP = EntityBase_m_HpMax(isMonster);
                            bool bShowEntityLayer = ShowEntity_bShowEntityLayer(isMonster);
                            auto _Position = ShowEntity_Position(isMonster);
                            int Distance = (int) Vector3::Distance(selfPos, _Position);
                            auto RootPosW2S = WorldToScreen(_Position);
                            auto RootPosVec2 = Vector2(glWidth- RootPosW2S.x, RootPosW2S.y);
                            if (RootPosW2S.z > 0) {
                                RootPosVec2 = Vector2(RootPosW2S.x, glHeight - RootPosW2S.y);
                            }
                            auto HeadPosVec2 = Vector2(RootPosVec2.X, RootPosVec2.Y - (glHeight / 6.35));


                            if (!m_bDeath && !m_IsDeathProgress) {

                                if (Esp.Monster.Visible) {
                                    if (bShowEntityLayer)continue;
                                }

                                if((MonsterType == 1 || MonsterType == 1)){

                                    if(Esp.Monster.MinionHealth) {
                                        draw_mini(ImVec2(RootPosVec2.X - (HP_40 / 2),RootPosVec2.Y + 54 + infobarMonster),CurHP,MaxHP,HP_40);
                                    }
                                    if(Esp.Monster.MinionDot){
                                        draw->AddCircleFilled( ImVec2(RootPosVec2.X,RootPosVec2.Y), 4, COL(RLine, 0, 0, 230));
                                        draw->AddCircleFilled( ImVec2(RootPosVec2.X,RootPosVec2.Y), 4, COL(RLine, 0, 0, 230));
                                    }
                                    if(Esp.Monster.MinionBox){
                                        ImVec2 v1 = ImVec2(RootPosVec2.X - (MiniBox / 2), RootPosVec2.Y);
                                        ImVec2 v2 = ImVec2(RootPosVec2.X + (MiniBox / 2), RootPosVec2.Y);
                                        ImVec2 v3 = ImVec2(RootPosVec2.X + (MiniBox / 2), RootPosVec2.Y - MiniBox);
                                        ImVec2 v4 = ImVec2(RootPosVec2.X - (MiniBox/ 2), RootPosVec2.Y - MiniBox);

                                        ImU32 clr = COL (RLine, GLine, BLine, Alpha);

                                        draw->AddLine( v1, v2,clr, SizeLine3);
                                        draw->AddLine( v2, v3,clr, SizeLine3);
                                        draw->AddLine( v3, v4,clr, SizeLine3);
                                        draw->AddLine( v4, v1,clr, SizeLine3);
                                    }
                                }


                                if((MonsterType == 2 || MonsterType == 5)){


                                if(Esp.Monster.Box) {
                                    ImVec2 v1 = ImVec2(RootPosVec2.X - (Box / 2), RootPosVec2.Y);
                                    ImVec2 v2 = ImVec2(RootPosVec2.X + (Box / 2), RootPosVec2.Y);
                                    ImVec2 v3 = ImVec2(RootPosVec2.X + (Box / 2), RootPosVec2.Y - Box);
                                    ImVec2 v4 = ImVec2(RootPosVec2.X - (Box / 2), RootPosVec2.Y - Box);

                                    ImU32 clr = COL (RLine, GLine, BLine, Alpha);

                                    draw->AddLine( v1, v2,clr, SizeLine);
                                    draw->AddLine( v2, v3,clr, SizeLine);
                                    draw->AddLine( v3, v4,clr, SizeLine);
                                    draw->AddLine( v4, v1,clr, SizeLine);
                                }
                                if (Esp.Monster.Health ) {
                                    ImVec2 textPos2(RootPosVec2.X + HpPosX,RootPosVec2.Y + HpPosY);

                                    if (( m_ID == 2002 || m_ID == 2003)) {
                                    //    ImVec4 color(RLine, GLine, BLine, Alpha);

                                        Draw_HpBoss(ImVec2(RootPosVec2.X - (HP_180 / 2),RootPosVec2.Y + 54), CurHP, MaxHP, HP_180);
                                    }else if((m_ID == 2011 || m_ID == 2013)){
                                        Draw_HpMonsGold(ImVec2(RootPosVec2.X - (HP_110 / 2), RootPosVec2.Y + 54), CurHP, MaxHP, HP_110);
                                    }else if((m_ID == 2056 || m_ID == 2072)){
                                        Draw_HpMonsCyan(ImVec2(RootPosVec2.X - (HP_110 / 2), RootPosVec2.Y + 54), CurHP, MaxHP, HP_110);
                                    }else if((m_ID == 2008 || m_ID == 2059 || m_ID == 2006 || m_ID == 2009)){
                                        Draw_HpMonsGreen(ImVec2(RootPosVec2.X - (HP_110 / 2), RootPosVec2.Y + 54), CurHP, MaxHP, HP_110);
                                    }else if((m_ID == 2004 || m_ID == 2004)){
                                        Draw_HpMonsRed(ImVec2(RootPosVec2.X - (HP_110 / 2), RootPosVec2.Y + 54), CurHP, MaxHP, HP_110);
                                    }else if((m_ID == 2072 || m_ID == 2056 || m_ID == 2012)){
                                        Draw_HpMons(ImVec2(RootPosVec2.X - (HP_80 / 2), RootPosVec2.Y + 54), CurHP, MaxHP, HP_80);
                                    }else{
                                        Draw_HpMons(ImVec2(RootPosVec2.X - (HP_110 / 2), RootPosVec2.Y + 54), CurHP, MaxHP, HP_110);
                                    }

                                    draw->AddText(NULL, 16, textPos2, IColor::Black(),std::to_string(CurHP).c_str());
                                    draw->AddText(NULL, 15, textPos2, IColor::Black(), std::to_string(CurHP).c_str());
                                    draw->AddText(NULL, 14, textPos2, IColor::Black(),std::to_string(CurHP).c_str());
                                    draw->AddText(NULL, 13, textPos2, IColor::White(), std::to_string(CurHP).c_str());
                                    draw->AddText(NULL, 13, textPos2, IColor::White(),std::to_string(CurHP).c_str());
                                    ImGui::GetForegroundDrawList()->PathStroke(IColor::Black(), ImDrawFlags_None, 3);
                              }
                                if (Esp.Monster.Name ) {
                                    std::string strName = MonsterToString(m_ID);
                                    auto textSize = ImGui::CalcTextSize2(strName.c_str(), 0, ((float) glHeight / 39.0f));
                                    draw->AddText(NULL, SizeText, ImVec2(RootPosVec2.X + PosisiMonster, RootPosVec2.Y + 37.0f), COL(RLine, GLine, BLine, 255), strName.c_str());
                                }
                                if (Esp.Monster.Alert) {
                                    if(bShowEntityLayer && Distance < 6.0f)continue;
                                    ImVec2 From = ImVec2(glWidth /2, (glHeight / 2) - 220);
                                    ImVec2 textPos1((glWidth /2 ) + 30, (glHeight / 2) - 207);
                                    ImVec2 textPos2((glWidth /2 ) - 11, (glHeight / 2) - 207);

                                    if (m_ID == 2002 && CurHP < MaxHP ) {
                                        std::string strAlert = "[ALERT] Lord is under attack!";
                                        auto textSize = ImGui::CalcTextSize2(strAlert.c_str(), 0, ((float) glHeight / 31.0f));
                                        draw->AddText(NULL, ((float) glHeight / 31.0f), {(float)(glWidth / 2) - (textSize.x / 2), (float)(glHeight - glHeight) + (float)(glHeight/ 10.7f)}, IM_COL32(RLine, GLine, BLine, 255), strAlert.c_str());
                                        Draw_HpBossAlert( ImVec2(glWidth /2 - (HP_300 / 2), (glHeight / 2) - 190), CurHP, MaxHP, HP_300);


                                        // DRAW TOTAL HP //
                                        draw->AddText(NULL, 13, textPos2, IM_COL32(RLine, GLine, BLine, 255),std::to_string(CurHP).append("/").c_str());
                                        draw->AddText(NULL, 13, textPos2, IM_COL32(RLine, GLine, BLine, 255),std::to_string(CurHP).append("/").c_str());
                                        // DRAW MAX HP //
                                        draw->AddText(NULL, 13, textPos1, IM_COL32(RLine, GLine, BLine, 255),std::to_string(MaxHP).c_str());
                                        draw->AddText(NULL, 13, textPos1, IM_COL32(RLine, GLine, BLine, 255),std::to_string(MaxHP).c_str());

                                    }
                                    if (m_ID == 2003 && CurHP < MaxHP ) {
                                        std::string strAlert = "[ALERT] Turtle is under attack!";
                                        auto textSize = ImGui::CalcTextSize2(strAlert.c_str(), 0, ((float) glHeight / 31.0f));
                                        draw->AddText(NULL, ((float) glHeight / 31.0f), {(float)(glWidth / 2) - (textSize.x / 2), (float)(glHeight - glHeight) + (float)(glHeight/ 10.7f)}, IM_COL32(RLine, GLine, BLine, 255), strAlert.c_str());
                                        Draw_HpBossAlert( ImVec2(glWidth /2 - (HP_300 / 2), (glHeight / 2) - 190), CurHP, MaxHP, HP_300);


                                        // DRAW TOTAL HP //
                                        draw->AddText(NULL, 13, textPos2, IM_COL32(RLine, GLine, BLine, 255),std::to_string(CurHP).append("/").c_str());
                                        draw->AddText(NULL, 13, textPos2, IM_COL32(RLine, GLine, BLine, 255),std::to_string(CurHP).append("/").c_str());
                                        // DRAW MAX HP //
                                        draw->AddText(NULL, 13, textPos1, IM_COL32(RLine, GLine, BLine, 255),std::to_string(MaxHP).c_str());
                                        draw->AddText(NULL, 13, textPos1, IM_COL32(RLine, GLine, BLine, 255),std::to_string(MaxHP).c_str());

                                    }
                                }
                             }

                            } } } }} } }}
void (*oSendGameReport)(void *unk);
void SendGameReport (void *unk){
    return;
}

static void *ReportMd5(void *thiz, bool bSendToServer) {
    void*(*RMd5) (void *m_BattleBridge, bool bSendToServer) = (void* (*)(void *,bool))(Methods["OffsetFunc_ReportMd5"]);
    return RMd5(thiz, bSendToServer);
}
void (*oPlugInTesting)(void *unk);
void PlugInTesting(void *unk) {
    return;
}
int m_kAPKSignature = 0;
bool (*oIsSignatureSame)(int kSignature);
bool IsSignatureSame(int kSignature) {
    kSignature = m_kAPKSignature;
    return true;
}

bool (*oOnMultiNetWorkOpen)(void *instance);
bool OnMultiNetWorkOpen(void *instance) {
    if (instance != NULL && iNetworkBoost) {
        return true;
    }
    return oOnMultiNetWorkOpen(instance);
}

bool (*oActiveComplete)(void *instance);
bool ActiveComplete(void *instance) {
    if (instance != NULL && iNetworkBoost) {
        return true;
    }
    return oActiveComplete(instance);
}

bool (*oOnMultiNetWorkHelp)(void *instance);
bool OnMultiNetWorkHelp(void *instance) {
    if (instance != NULL && iNetworkBoost) {
        return true;
    }
    return oOnMultiNetWorkHelp(instance);
}
bool (*oUnlockFreeSummon)(void *instance);
bool UnlockFreeSummon(void *instance) {
    if (instance != NULL && freeSpell) {
        return true;
    }
    return oUnlockFreeSummon(instance);
}
void (*oNotifyerror)(void *unk);
void Notifyerror(void *unk) {
    return;
}
void (*oNotifyerrorLog)(void *unk,bool bDebugReport, string strLog, string stackTrace);
void NotifyerrorLog(void *unk,bool bDebugReport, string strLog, string stackTrace) {
    return;
}