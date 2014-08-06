package com.doteyplay.game.service.bo.gateway;

import java.util.List;

import com.doteyplay.core.bhns.AbstractSimpleService;
import com.doteyplay.game.domain.common.SessionHolder;
import com.doteyplay.game.service.runtime.GlobalSessionCache;
import com.doteyplay.net.message.AbstractMessage;
import com.doteyplay.net.message.DefaultMessage;

public class NSGameGateWayService extends
		AbstractSimpleService<IGameGateWayService> implements
		IGameGateWayService
{

	@Override
	public void kick(long userId,boolean worldLogout)
	{
		GlobalSessionCache.getInstance().disconnect(userId,worldLogout);
	}
	
	@Override
	public void kickAll()
	{
		GlobalSessionCache.getInstance().disconnectAll();
	}

	@Override
	public int getPortalId()
	{
		return PORTAL_ID;
	}

	@Override
	public void loginNotify(long userId, long sessionId)
	{

	}

//	public void messageReceived(byte[] messageBytes, long sessionId)
//	{
//		throw new UnsupportedOperationException(
//				"gateway service unsuppot message recv");
//	}

	@Override
	public void broadcastMessage(List<Long> roleIds, AbstractMessage message)
	{
		for (long roleId : roleIds)
		{
			SessionHolder holder = GlobalSessionCache.getInstance()
					.getByRoleId(roleId);
			if (holder != null)
				holder.sendMsg(message);
		}
	}

	@Override
	public void sendMessage(byte[] messageBytes, long sessionId)
	{
		DefaultMessage message = new DefaultMessage();
		message.setData(messageBytes);
		GlobalSessionCache.getInstance().sendMessageBySessionId(message, sessionId);
	}

	@Override
	public void sendMessage(byte[] messageBytes)
	{

	}

}